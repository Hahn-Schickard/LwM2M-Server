#include "Resource.hpp"
#include "TestRequester.hpp"
#include "Variant_Visitor.hpp"

#include "../MockExceptionHandler.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <memory>
#include <thread>
#include <variant>

using namespace std;
using namespace LwM2M;

// NOLINTBEGIN
struct ResourceExpectations {
  TestRequesterPtr requester;
  ResourceDescriptorPtr descriptor;
  DataFormat result;

  ResourceExpectations(ResourceDescriptorPtr descriptor, DataFormat result)
      : requester(make_shared<TestRequester>()), descriptor(move(descriptor)),
        result(move(result)) {}

  DataVariant get() { return result.get(descriptor->data_type_); }
};

using ResourceExpectationsPtr = shared_ptr<ResourceExpectations>;

using ResourceTestParameter = tuple<ResourceDescriptorPtr, DataFormat>;

template <typename T> struct RespondWithDelay {
  TestRequesterPtr requester;
  int delay;
  T response;

  RespondWithDelay(
      TestRequesterPtr requester, int delay_value, T response_value)
      : requester(move(requester)), delay(delay_value),
        response(move(response_value)) {}

  void operator()() {
    this_thread::sleep_for(chrono::milliseconds(delay));
    requester->respond(response);
  }
};

ResourcePtr makeTested(const MockExceptionHandlerPtr& exception_handler,
    const TestRequesterPtr& requester,
    const ResourceDescriptorPtr& descriptor) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 86544); // NOLINT

  function<void(exception_ptr)> exception_handler_cb =
      bind(&ExceptionHandlerInterface::handleDeviceException, exception_handler,
          placeholders::_1);
  return NonemptyPointer::make_shared<Resource>(exception_handler_cb, requester,
      endpoint, descriptor, ElementID(0, 0, 0));
}

class ResourceTest : public testing::TestWithParam<ResourceTestParameter> {
protected:
  void SetUp() override {
    expected_ = make_shared<ResourceExpectations>(
        get<0>(GetParam()), get<1>(GetParam()));
    tested_ = makeTested(
        exception_handler_, expected_->requester, expected_->descriptor);
  }

  void TearDown() override {
    expected_.reset();
    exception_handler_.reset();
  }

  ResourcePtr tested_ = NonemptyPointer::make_shared<Resource>();
  ResourceExpectationsPtr expected_;
  MockExceptionHandlerPtr exception_handler_ =
      std::make_shared<MockExceptionHandler>(); // NOLINT
  int response_delay_ms_ = 1;
};

void compareVariant(const DataVariant& tested, const DataVariant& expected) {
  match(
      tested, [&](bool value) { EXPECT_EQ(value, get<bool>(expected)); },
      [&](int64_t value) { EXPECT_EQ(value, get<int64_t>(expected)); },
      [&](uint64_t value) { EXPECT_EQ(value, get<uint64_t>(expected)); },
      [&](double value) { EXPECT_EQ(value, get<double>(expected)); },
      [&](const string& value) { EXPECT_EQ(value, get<string>(expected)); },
      [&](TimeStamp tested_timestamp) {
        auto expected_timestamp = get<TimeStamp>(expected);
        EXPECT_EQ(tested_timestamp.getValue(), expected_timestamp.getValue());
        EXPECT_EQ(tested_timestamp.toString(), expected_timestamp.toString());
      },
      [&](ObjectLink tested_link) {
        auto expected_link = get<ObjectLink>(expected);
        EXPECT_EQ(tested_link.object_id_, expected_link.object_id_);
        EXPECT_EQ(tested_link.instance_id_, expected_link.instance_id_);
        EXPECT_EQ(tested_link.toString(), expected_link.toString());
      },
      [&](const vector<uint8_t>& value) {
        EXPECT_EQ(value, get<vector<uint8_t>>(expected));
      });
}

void processReadable(ReadablePtr readable,
    const ResourceExpectationsPtr& expected, int response_delay_ms) {
  auto result = readable->read();
  auto finished = async(std::launch::async,
      RespondWithDelay<DataFormat>(
          expected->requester, response_delay_ms, expected->result));
  if (finished.wait_for(1s) != future_status::ready) {
    FAIL() << "Async Read of resource " << expected->descriptor->id_ << ":"
           << expected->descriptor->name_ << " has timed-out" << endl;
    std::terminate();
  }
  compareVariant(result.get(), expected->get());
  finished.get();
}

void readResource(ResourcePtr resource, ResourceExpectationsPtr expected,
    int response_delay_ms) {
  auto instance = resource->getResourceInstance();
  match(
      instance,
      [&](ReadablePtr& value) {
        processReadable(value, expected, response_delay_ms);
      },
      [&](ReadAndWritablePtr& value) {
        processReadable(value, expected, response_delay_ms);
      },
      [&](auto) {
        auto error_msg = "Resource " + resource->getDescriptor()->name_ +
            " ID " + to_string(resource->getDescriptor()->id_) +
            " is not readable.";
        throw logic_error(error_msg);
      });
}

// NOLINTNEXTLINE
TEST_P(ResourceTest, canReadValue) {
  if (expected_->descriptor->operations_ == OperationsType::READ ||
      expected_->descriptor->operations_ == OperationsType::READ_AND_WRITE) {
    EXPECT_NO_THROW(readResource(tested_, expected_, response_delay_ms_));
  } else {
    EXPECT_THROW(
        { readResource(tested_, expected_, response_delay_ms_); }, logic_error);
  }
}

void cancelReadable(ReadablePtr readable) {
  auto result = readable->read();
  result.cancel();
  EXPECT_THROW(result.get(), runtime_error);
}

void cancelReadResource(ResourcePtr resource) {
  auto instance = resource->getResourceInstance();
  match(
      instance, [&](ReadablePtr& value) { cancelReadable(value); },
      [&](ReadAndWritablePtr& value) { cancelReadable(value); },
      [&](auto) {
        auto error_msg = "Resource " + resource->getDescriptor()->name_ +
            " ID " + to_string(resource->getDescriptor()->id_) +
            " is not readable.";
        throw logic_error(error_msg);
      });
}

// NOLINTNEXTLINE
TEST_P(ResourceTest, canCancelRead) {
  if (expected_->descriptor->operations_ == OperationsType::READ ||
      expected_->descriptor->operations_ == OperationsType::READ_AND_WRITE) {
    EXPECT_NO_THROW(cancelReadResource(tested_));
  } else {
    EXPECT_THROW({ cancelReadResource(tested_); }, logic_error);
  }
}

void processWritable(WritablePtr writable,
    const ResourceExpectationsPtr& expected, int response_delay_ms) {
  auto result = writable->write(expected->get());
  auto finished = async(std::launch::async,
      RespondWithDelay<bool>(expected->requester, response_delay_ms, true));
  if (finished.wait_for(1s) != future_status::ready) {
    FAIL() << "Async Write of resource " << expected->descriptor->id_ << ":"
           << expected->descriptor->name_ << " has timed-out" << endl;
    std::terminate();
  }
  EXPECT_TRUE(result.get());
  finished.get();
}

void writeResource(ResourcePtr resource, ResourceExpectationsPtr expected,
    int response_delay_ms) {
  auto instance = resource->getResourceInstance();
  match(
      instance,
      [&](WritablePtr& value) {
        processWritable(value, expected, response_delay_ms);
      },
      [&](ReadAndWritablePtr& value) {
        processWritable(value, expected, response_delay_ms);
      },
      [&](auto) {
        auto error_msg = "Resource " + resource->getDescriptor()->name_ +
            " ID " + to_string(resource->getDescriptor()->id_) +
            " is not writable.";
        throw logic_error(error_msg);
      });
}

// NOLINTNEXTLINE
TEST_P(ResourceTest, canWriteValue) {
  if (expected_->descriptor->operations_ == OperationsType::WRITE ||
      expected_->descriptor->operations_ == OperationsType::READ_AND_WRITE) {
    EXPECT_NO_THROW(writeResource(tested_, expected_, response_delay_ms_));
  } else {
    EXPECT_THROW({ writeResource(tested_, expected_, response_delay_ms_); },
        logic_error);
  }
}

void cancelWritable(WritablePtr writable) {
  auto result = writable->write(true);
  result.cancel();
  EXPECT_THROW(result.get(), runtime_error);
}

void cancelWriteResource(ResourcePtr resource) {
  auto instance = resource->getResourceInstance();
  match(
      instance, [&](WritablePtr& value) { cancelWritable(value); },
      [&](ReadAndWritablePtr& value) { cancelWritable(value); },
      [&](auto) {
        auto error_msg = "Resource " + resource->getDescriptor()->name_ +
            " ID " + to_string(resource->getDescriptor()->id_) +
            " is not writable.";
        throw logic_error(error_msg);
      });
}

// NOLINTNEXTLINE
TEST_P(ResourceTest, canCancelWrite) {
  if (expected_->descriptor->operations_ == OperationsType::WRITE ||
      expected_->descriptor->operations_ == OperationsType::READ_AND_WRITE) {
    EXPECT_NO_THROW(cancelWriteResource(tested_));
  } else {
    EXPECT_THROW({ cancelWriteResource(tested_); }, logic_error);
  }
}

void processExecutable(ExecutablePtr resource,
    const ResourceExpectationsPtr& expected, int response_delay_ms) {
  auto result = resource->execute("");
  auto finished = async(std::launch::async,
      RespondWithDelay<bool>(expected->requester, response_delay_ms, true));
  if (finished.wait_for(1s) != future_status::ready) {
    FAIL() << "Async Execute of resource " << expected->descriptor->id_ << ":"
           << expected->descriptor->name_ << " has timed-out" << endl;
    std::terminate();
  }
  EXPECT_TRUE(result.get());
  finished.get();
}

void executeResource(ResourcePtr resource, ResourceExpectationsPtr expected,
    int response_delay_ms) {
  auto instance = resource->getResourceInstance();
  match(
      instance,
      [&](ExecutablePtr& value) {
        processExecutable(value, expected, response_delay_ms);
      },
      [&](auto) {
        auto error_msg = "Resource " + resource->getDescriptor()->name_ +
            " ID " + to_string(resource->getDescriptor()->id_) +
            " is not executable.";
        throw logic_error(error_msg);
      });
}

// NOLINTNEXTLINE
TEST_P(ResourceTest, canExecuteAction) {
  if (expected_->descriptor->operations_ == OperationsType::EXECUTE) {
    EXPECT_NO_THROW(executeResource(tested_, expected_, response_delay_ms_));
  } else {
    EXPECT_THROW({ executeResource(tested_, expected_, response_delay_ms_); },
        logic_error);
  }
}

void cancelExecutable(ExecutablePtr excecutable) {
  auto result = excecutable->execute("");
  result.cancel();
  EXPECT_THROW(result.get(), runtime_error);
}

void cancelExecuteResource(ResourcePtr resource) {
  auto instance = resource->getResourceInstance();
  match(
      instance, [&](ExecutablePtr& value) { cancelExecutable(value); },
      [&](auto) {
        auto error_msg = "Resource " + resource->getDescriptor()->name_ +
            " ID " + to_string(resource->getDescriptor()->id_) +
            " is not executable.";
        throw logic_error(error_msg);
      });
}

// NOLINTNEXTLINE
TEST_P(ResourceTest, canCancelExecutable) {
  if (expected_->descriptor->operations_ == OperationsType::EXECUTE) {
    EXPECT_NO_THROW(cancelExecuteResource(tested_));
  } else {
    EXPECT_THROW({ cancelExecuteResource(tested_); }, logic_error);
  }
}

struct GenerateTestName {
  string operator()(
      const testing::TestParamInfo<ResourceTestParameter>& parameter) const {
    const auto& descriptor = get<0>(parameter.param);
    auto name = descriptor->name_ + toString(descriptor->data_type_) +
        toString(descriptor->operations_);
    name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
    name.erase(remove_if(name.begin(), name.end(),
                   [](unsigned char x) { return x == '_'; }),
        name.end());
    return name;
  }
};

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(ResourceTests, ResourceTest,
    testing::Values(make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                   OperationsType::READ, false, true,
                                   DataType::BOOLEAN, "", ""),
                        DataFormat(DataVariant((bool)true))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::WRITE,
                false, true, DataType::BOOLEAN, "", ""),
            DataFormat(DataVariant((bool)true))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                       OperationsType::READ_AND_WRITE, false, true,
                       DataType::BOOLEAN, "", ""),
            DataFormat(DataVariant((bool)true))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::EXECUTE,
                false, true, DataType::BOOLEAN, "", ""),
            DataFormat(DataVariant((bool)true))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::READ,
                false, true, DataType::FLOAT, "", ""),
            DataFormat(DataVariant((double)52.4))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::WRITE,
                false, true, DataType::FLOAT, "", ""),
            DataFormat(DataVariant((double)20.2))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                       OperationsType::READ_AND_WRITE, false, true,
                       DataType::FLOAT, "", ""),
            DataFormat(DataVariant((double)20.2))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::EXECUTE,
                false, true, DataType::FLOAT, "", ""),
            DataFormat(DataVariant((double)20.2))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::READ,
                false, true, DataType::OBJECT_LINK, "", ""),
            DataFormat(DataVariant(ObjectLink(0, 0)))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::WRITE,
                false, true, DataType::OBJECT_LINK, "", ""),
            DataFormat(DataVariant(ObjectLink(0, 0)))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                       OperationsType::READ_AND_WRITE, false, true,
                       DataType::OBJECT_LINK, "", ""),
            DataFormat(DataVariant(ObjectLink(0, 0)))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::EXECUTE,
                false, true, DataType::OBJECT_LINK, "", ""),
            DataFormat(DataVariant(ObjectLink(0, 0)))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::READ,
                false, true, DataType::OPAQUE, "", ""),
            DataFormat(DataVariant(vector<uint8_t>{1, 2, 3, 4, 5}))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::WRITE,
                false, true, DataType::OPAQUE, "", ""),
            DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                       OperationsType::READ_AND_WRITE, false, true,
                       DataType::OPAQUE, "", ""),
            DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::EXECUTE,
                false, true, DataType::OPAQUE, "", ""),
            DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::READ,
                false, true, DataType::SIGNED_INTEGER, "", ""),
            DataFormat(DataVariant((int64_t)-100))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::WRITE,
                false, true, DataType::SIGNED_INTEGER, "", ""),
            DataFormat(DataVariant((int64_t)-100))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                       OperationsType::READ_AND_WRITE, false, true,
                       DataType::SIGNED_INTEGER, "", ""),
            DataFormat(DataVariant((int64_t)-100))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::EXECUTE,
                false, true, DataType::SIGNED_INTEGER, "", ""),
            DataFormat(DataVariant((int64_t)-100))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::READ,
                false, true, DataType::STRING, "", ""),
            DataFormat(DataVariant((string) "Hello"))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::WRITE,
                false, true, DataType::STRING, "", ""),
            DataFormat(DataVariant(string("hello")))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                       OperationsType::READ_AND_WRITE, false, true,
                       DataType::STRING, "", ""),
            DataFormat(DataVariant(string("hello")))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::EXECUTE,
                false, true, DataType::STRING, "", ""),
            DataFormat(DataVariant(string("hello")))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::READ,
                false, true, DataType::UNSIGNED_INTEGER, "", ""),
            DataFormat(DataVariant((uint64_t)26))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::WRITE,
                false, true, DataType::UNSIGNED_INTEGER, "", ""),
            DataFormat(DataVariant((uint64_t)26))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                       OperationsType::READ_AND_WRITE, false, true,
                       DataType::UNSIGNED_INTEGER, "", ""),
            DataFormat(DataVariant((uint64_t)26))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::EXECUTE,
                false, true, DataType::UNSIGNED_INTEGER, "", ""),
            DataFormat(DataVariant((uint64_t)26))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::READ,
                false, true, DataType::TIME, "", ""),
            DataFormat(DataVariant((uint64_t)12850912328012))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::WRITE,
                false, true, DataType::TIME, "", ""),
            DataFormat(DataVariant((uint64_t)12850912328012))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                       OperationsType::READ_AND_WRITE, false, true,
                       DataType::TIME, "", ""),
            DataFormat(DataVariant((uint64_t)12850912328012))),
        make_tuple(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::EXECUTE,
                false, true, DataType::TIME, "", ""),
            DataFormat(DataVariant((uint64_t)12850912328012)))),
    GenerateTestName());

// NOLINTEND
