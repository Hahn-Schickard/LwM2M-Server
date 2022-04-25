#include "Executable.hpp"
#include "ReadAndWritable.hpp"
#include "Readable.hpp"
#include "TestRequester.hpp"
#include "Variant_Visitor.hpp"
#include "Writable.hpp"

#include "../MockExceptionHandler.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <memory>
#include <thread>
#include <variant>

template <typename T> std::string getTypeName(T type) {
  int status;
  std::string type_name = typeid(type).name();
  char *demangled_name =
      abi::__cxa_demangle(type_name.c_str(), NULL, NULL, &status);
  if (status == 0) {
    type_name = demangled_name;
    type_name.erase(std::remove(type_name.begin(), type_name.end(), '*'),
                    type_name.end());
    std::free(demangled_name);
  }
  return type_name;
}

template <typename T> bool isEmptyPtr(std::shared_ptr<T> arg) {
  if (arg) {
    return true;
  } else {
    std::string error_msg = getTypeName(arg) + " can not be empty";
    throw std::invalid_argument(error_msg);
  }
}

template <typename T, typename... Types>
bool isEmptyPtr(std::shared_ptr<T> arg, Types... args) {
  if (arg) {
    return isEmptyPtr(args...);
  } else {
    std::string error_msg = getTypeName(arg) + " can not be empty";
    throw std::invalid_argument(error_msg);
  }
}

using namespace std;
using namespace LwM2M;

using ResourceVariant =
    variant<ResourcePtr<bool>, ResourcePtr<int64_t>, ResourcePtr<uint64_t>,
            ResourcePtr<double>, ResourcePtr<string>, ResourcePtr<ObjectLink>,
            ResourcePtr<vector<uint8_t>>>;

struct ResourceExpectations {
  TestRequesterPtr requester_;
  ResourceDescriptorPtr descriptor_;
  DataFormat result_;

  ResourceExpectations(ResourceDescriptorPtr descriptor, DataFormat result)
      : requester_(make_shared<TestRequester>()), descriptor_(descriptor),
        result_(result) {}
};

using ResourceExpectationsPtr = shared_ptr<ResourceExpectations>;

using ResourceTestParameter = tuple<ResourceDescriptorPtr, DataFormat>;

template <typename T> struct RespondWithDelay {
  TestRequesterPtr requester_;
  int delay_;
  T response_;

  RespondWithDelay(TestRequesterPtr requester, int delay, T response)
      : requester_(requester), delay_(delay), response_(response) {}

  void operator()() {
    this_thread::sleep_for(chrono::milliseconds(delay_));
    requester_->respond(response_);
  }
};

template <typename T>
ResourceVariant makeTestResource(MockExceptionHandlerPtr exception_handler,
                                 TestRequesterPtr requester,
                                 EndpointPtr endpoint, ElementID parent,
                                 ResourceDescriptorPtr descriptor) {
  if (isEmptyPtr(exception_handler, requester, endpoint, descriptor)) {
    function<void(exception_ptr)> exception_handler_cb =
        bind(&ExceptionHandlerInterface::handleDeviceException,
             exception_handler, placeholders::_1);
    switch (descriptor->operations_) {
    case OperationsType::READ: {
      return ResourceVariant(make_shared<Readable<T>>(
          descriptor, exception_handler_cb, requester, endpoint, parent));
    }
    case OperationsType::READ_AND_WRITE: {
      return ResourceVariant(make_shared<ReadAndWritable<T>>(
          descriptor, exception_handler_cb, requester, endpoint, parent));
    }
    case OperationsType::WRITE: {
      return ResourceVariant(
          make_shared<Writable<T>>(descriptor, requester, endpoint, parent));
    }
    case OperationsType::EXECUTE: {
      return ResourceVariant(
          make_shared<Executable<T>>(descriptor, requester, endpoint, parent));
    }
    default: {
      throw invalid_argument("Resource operation type is invalid. Unit tests "
                             "only support valid resources!");
    }
    }
  } else {
    throw logic_error(
        "Check for empty ptr return false, but did not throw an exception");
  }
}

ResourceVariant makeTested(MockExceptionHandlerPtr exception_handler,
                           TestRequesterPtr requester,
                           ResourceDescriptorPtr descriptor) {
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 86544);

  ResourceVariant resource;
  switch (descriptor->data_type_) {
  case DataType::BOOLEAN: {
    return resource =
               makeTestResource<bool>(exception_handler, requester, endpoint,
                                      ElementID(0, 0, 0), descriptor);
  }
  case DataType::SIGNED_INTEGER: {
    return resource =
               makeTestResource<int64_t>(exception_handler, requester, endpoint,
                                         ElementID(0, 0, 0), descriptor);
  }
  case DataType::UNSIGNED_INTEGER:
  case DataType::TIME: {
    return resource = makeTestResource<uint64_t>(exception_handler, requester,
                                                 endpoint, ElementID(0, 0, 0),
                                                 descriptor);
  }
  case DataType::FLOAT: {
    return resource =
               makeTestResource<double>(exception_handler, requester, endpoint,
                                        ElementID(0, 0, 0), descriptor);
  }
  case DataType::STRING: {
    return resource =
               makeTestResource<string>(exception_handler, requester, endpoint,
                                        ElementID(0, 0, 0), descriptor);
  }
  case DataType::OPAQUE: {
    return resource = makeTestResource<vector<uint8_t>>(
               exception_handler, requester, endpoint, ElementID(0, 0, 0),
               descriptor);
  }
  case DataType::OBJECT_LINK: {
    return resource = makeTestResource<ObjectLink>(exception_handler, requester,
                                                   endpoint, ElementID(0, 0, 0),
                                                   descriptor);
  }
  default: {
    throw invalid_argument("Resource data type is invalid. Unit tests "
                           "only support valid resources!");
  }
  }
}

class ResourceTest : public testing::TestWithParam<ResourceTestParameter> {
protected:
  void SetUp() override {
    expected_ = make_shared<ResourceExpectations>(get<0>(GetParam()),
                                                  get<1>(GetParam()));
    tested_ = makeTested(exception_handler_, expected_->requester_,
                         expected_->descriptor_);
  }

  void TearDown() override {
    expected_.reset();
    match(
        tested_, [&](ResourcePtr<bool> resource) { resource.reset(); },
        [&](ResourcePtr<int64_t> resource) { resource.reset(); },
        [&](ResourcePtr<uint64_t> resource) { resource.reset(); },
        [&](ResourcePtr<double> resource) { resource.reset(); },
        [&](ResourcePtr<string> resource) { resource.reset(); },
        [&](ResourcePtr<ObjectLink> resource) { resource.reset(); },
        [&](ResourcePtr<vector<uint8_t>> resource) { resource.reset(); });
    exception_handler_.reset();
  }

  ResourceVariant tested_;
  ResourceExpectationsPtr expected_;
  MockExceptionHandlerPtr exception_handler_ =
      std::make_shared<MockExceptionHandler>();
  int response_delay_ms_ = 1;
};

TEST_P(ResourceTest, isValidResourceVariant) {
  ASSERT_FALSE(tested_.valueless_by_exception());
}

template <typename T>
void readResource(ResourcePtr<T> resource, ResourceExpectationsPtr expected,
                  int response_delay_ms) {
  try {
    auto result = resource->read();
    auto expected_result = expected->result_;
    auto finished =
        async(std::launch::async,
              RespondWithDelay<DataFormat>(expected->requester_,
                                           response_delay_ms, expected_result));
    if (finished.wait_for(1s) != future_status::ready) {
      FAIL() << "Async Read of resource " << resource->getDescriptor()->id_
             << ":" << resource->getDescriptor()->name_ << " has timed-out"
             << endl;
      std::terminate();
    }
    EXPECT_EQ(result.get(), expected_result.get<T>());
    finished.get();
  } catch (exception &ex) {
    FAIL() << "Caught exception: " << ex.what() << endl;
  }
}

TEST_P(ResourceTest, canReadValue) {
  if (expected_->descriptor_->operations_ == OperationsType::READ ||
      expected_->descriptor_->operations_ == OperationsType::READ_AND_WRITE) {
    match(
        tested_,
        [&](ResourcePtr<bool> resource) {
          readResource<bool>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<int64_t> resource) {
          readResource<int64_t>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<uint64_t> resource) {
          readResource<uint64_t>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<double> resource) {
          readResource<double>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<string> resource) {
          readResource<string>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<ObjectLink> resource) {
          readResource<ObjectLink>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<vector<uint8_t>> resource) {
          readResource<vector<uint8_t>>(resource, expected_,
                                        response_delay_ms_);
        });
  } else {
    match(
        tested_,
        [&](ResourcePtr<bool> resource) {
          EXPECT_THROW({ resource->read(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<int64_t> resource) {
          EXPECT_THROW({ resource->read(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<uint64_t> resource) {
          EXPECT_THROW({ resource->read(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<double> resource) {
          EXPECT_THROW({ resource->read(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<string> resource) {
          EXPECT_THROW({ resource->read(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<ObjectLink> resource) {
          EXPECT_THROW({ resource->read(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<vector<uint8_t>> resource) {
          EXPECT_THROW({ resource->read(); }, UnsupportedMethod);
        });
  }
}

template <typename T>
void writeResource(ResourcePtr<T> resource, ResourceExpectationsPtr expected,
                   int response_delay_ms) {
  try {
    auto result = resource->write(expected->result_.data_);

    auto finished = async(
        std::launch::async,
        RespondWithDelay<bool>(expected->requester_, response_delay_ms, true));
    if (finished.wait_for(1s) != future_status::ready) {
      FAIL() << "Async Write of resource " << resource->getDescriptor()->id_
             << ":" << resource->getDescriptor()->name_ << " has timed-out"
             << endl;
      std::terminate();
    }
    EXPECT_TRUE(result.get());
    finished.get();
  } catch (exception &ex) {
    FAIL() << "Caught exception: " << ex.what() << endl;
  }
}

TEST_P(ResourceTest, canWriteValue) {
  if (expected_->descriptor_->operations_ == OperationsType::WRITE ||
      expected_->descriptor_->operations_ == OperationsType::READ_AND_WRITE) {
    match(
        tested_,
        [&](ResourcePtr<bool> resource) {
          writeResource<bool>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<int64_t> resource) {
          writeResource<int64_t>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<uint64_t> resource) {
          writeResource<uint64_t>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<double> resource) {
          writeResource<double>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<string> resource) {
          writeResource<string>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<ObjectLink> resource) {
          writeResource<ObjectLink>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<vector<uint8_t>> resource) {
          writeResource<vector<uint8_t>>(resource, expected_,
                                         response_delay_ms_);
        });
  } else {
    match(
        tested_,
        [&](ResourcePtr<bool> resource) {
          EXPECT_THROW({ resource->write(DataVariant()); }, UnsupportedMethod);
        },
        [&](ResourcePtr<int64_t> resource) {
          EXPECT_THROW({ resource->write(DataVariant()); }, UnsupportedMethod);
        },
        [&](ResourcePtr<uint64_t> resource) {
          EXPECT_THROW({ resource->write(DataVariant()); }, UnsupportedMethod);
        },
        [&](ResourcePtr<double> resource) {
          EXPECT_THROW({ resource->write(DataVariant()); }, UnsupportedMethod);
        },
        [&](ResourcePtr<string> resource) {
          EXPECT_THROW({ resource->write(DataVariant()); }, UnsupportedMethod);
        },
        [&](ResourcePtr<ObjectLink> resource) {
          EXPECT_THROW({ resource->write(DataVariant()); }, UnsupportedMethod);
        },
        [&](ResourcePtr<vector<uint8_t>> resource) {
          EXPECT_THROW({ resource->write(DataVariant()); }, UnsupportedMethod);
        });
  }
}

template <typename T>
void executeResource(ResourcePtr<T> resource, ResourceExpectationsPtr expected,
                     int response_delay_ms) {
  try {
    auto result = resource->execute();

    auto finished = async(
        std::launch::async,
        RespondWithDelay<bool>(expected->requester_, response_delay_ms, true));
    if (finished.wait_for(1s) != future_status::ready) {
      FAIL() << "Async Execute of resource " << resource->getDescriptor()->id_
             << ":" << resource->getDescriptor()->name_ << " has timed-out"
             << endl;
      std::terminate();
    }
    EXPECT_TRUE(result.get());
    finished.get();
  } catch (exception &ex) {
    FAIL() << "Caught exception: " << ex.what() << endl;
  }
}

TEST_P(ResourceTest, canExecuteAction) {
  if (expected_->descriptor_->operations_ == OperationsType::EXECUTE) {
    match(
        tested_,
        [&](ResourcePtr<bool> resource) {
          executeResource<bool>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<int64_t> resource) {
          executeResource<int64_t>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<uint64_t> resource) {
          executeResource<uint64_t>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<double> resource) {
          executeResource<double>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<string> resource) {
          executeResource<string>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<ObjectLink> resource) {
          executeResource<ObjectLink>(resource, expected_, response_delay_ms_);
        },
        [&](ResourcePtr<vector<uint8_t>> resource) {
          executeResource<vector<uint8_t>>(resource, expected_,
                                           response_delay_ms_);
        });
  } else {
    match(
        tested_,
        [&](ResourcePtr<bool> resource) {
          EXPECT_THROW({ resource->execute(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<int64_t> resource) {
          EXPECT_THROW({ resource->execute(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<uint64_t> resource) {
          EXPECT_THROW({ resource->execute(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<double> resource) {
          EXPECT_THROW({ resource->execute(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<string> resource) {
          EXPECT_THROW({ resource->execute(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<ObjectLink> resource) {
          EXPECT_THROW({ resource->execute(); }, UnsupportedMethod);
        },
        [&](ResourcePtr<vector<uint8_t>> resource) {
          EXPECT_THROW({ resource->execute(); }, UnsupportedMethod);
        });
  }
}

struct GenerateTestName {
  string operator()(
      const testing::TestParamInfo<ResourceTestParameter> &parameter) const {
    auto descriptor = get<0>(parameter.param);
    auto name = descriptor->name_ + toString(descriptor->data_type_) +
                toString(descriptor->operations_);
    name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
    name.erase(
        remove_if(name.begin(), name.end(),
                  [](unsigned char x) { return x == '_' ? true : false; }),
        name.end());
    return name;
  }
};

INSTANTIATE_TEST_SUITE_P(
    ResourceTests, ResourceTest,
    testing::Values(
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::READ, false,
                                                   true, DataType::BOOLEAN, "",
                                                   ""),
                   DataFormat(DataVariant((bool)true))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::WRITE, false,
                                                   true, DataType::BOOLEAN, "",
                                                   ""),
                   DataFormat(DataVariant((bool)true))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::READ_AND_WRITE, false, true,
                       DataType::BOOLEAN, "", ""),
                   DataFormat(DataVariant((bool)true))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::EXECUTE,
                                                   false, true,
                                                   DataType::BOOLEAN, "", ""),
                   DataFormat(DataVariant((bool)true))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::READ, false,
                                                   true, DataType::FLOAT, "",
                                                   ""),
                   DataFormat(DataVariant((double)52.4))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::WRITE, false,
                                                   true, DataType::FLOAT, "",
                                                   ""),
                   DataFormat(DataVariant((double)20.2))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::READ_AND_WRITE, false, true,
                       DataType::FLOAT, "", ""),
                   DataFormat(DataVariant((double)20.2))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::EXECUTE,
                                                   false, true, DataType::FLOAT,
                                                   "", ""),
                   DataFormat(DataVariant((double)20.2))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::READ, false,
                                                   true, DataType::OBJECT_LINK,
                                                   "", ""),
                   DataFormat(DataVariant(ObjectLink(0, 0)))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::WRITE, false,
                                                   true, DataType::OBJECT_LINK,
                                                   "", ""),
                   DataFormat(DataVariant(ObjectLink(0, 0)))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::READ_AND_WRITE, false, true,
                       DataType::OBJECT_LINK, "", ""),
                   DataFormat(DataVariant(ObjectLink(0, 0)))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::EXECUTE, false, true,
                       DataType::OBJECT_LINK, "", ""),
                   DataFormat(DataVariant(ObjectLink(0, 0)))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::READ, false,
                                                   true, DataType::OPAQUE, "",
                                                   ""),
                   DataFormat(DataVariant(vector<uint8_t>{1, 2, 3, 4, 5}))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::WRITE, false,
                                                   true, DataType::OPAQUE, "",
                                                   ""),
                   DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::READ_AND_WRITE, false, true,
                       DataType::OPAQUE, "", ""),
                   DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::EXECUTE,
                                                   false, true,
                                                   DataType::OPAQUE, "", ""),
                   DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::READ, false, true,
                       DataType::SIGNED_INTEGER, "", ""),
                   DataFormat(DataVariant((int64_t)-100))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::WRITE, false, true,
                       DataType::SIGNED_INTEGER, "", ""),
                   DataFormat(DataVariant((int64_t)-100))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::READ_AND_WRITE, false, true,
                       DataType::SIGNED_INTEGER, "", ""),
                   DataFormat(DataVariant((int64_t)-100))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::EXECUTE, false, true,
                       DataType::SIGNED_INTEGER, "", ""),
                   DataFormat(DataVariant((int64_t)-100))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::READ, false,
                                                   true, DataType::STRING, "",
                                                   ""),
                   DataFormat(DataVariant((string) "Hello"))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::WRITE, false,
                                                   true, DataType::STRING, "",
                                                   ""),
                   DataFormat(DataVariant(string("hello")))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::READ_AND_WRITE, false, true,
                       DataType::STRING, "", ""),
                   DataFormat(DataVariant(string("hello")))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::EXECUTE,
                                                   false, true,
                                                   DataType::STRING, "", ""),
                   DataFormat(DataVariant(string("hello")))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::READ, false, true,
                       DataType::UNSIGNED_INTEGER, "", ""),
                   DataFormat(DataVariant((uint64_t)26))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::WRITE, false, true,
                       DataType::UNSIGNED_INTEGER, "", ""),
                   DataFormat(DataVariant((uint64_t)26))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::READ_AND_WRITE, false, true,
                       DataType::UNSIGNED_INTEGER, "", ""),
                   DataFormat(DataVariant((uint64_t)26))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::EXECUTE, false, true,
                       DataType::UNSIGNED_INTEGER, "", ""),
                   DataFormat(DataVariant((uint64_t)26))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::READ, false,
                                                   true, DataType::TIME, "",
                                                   ""),
                   DataFormat(DataVariant((uint64_t)12850912328012))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::WRITE, false,
                                                   true, DataType::TIME, "",
                                                   ""),
                   DataFormat(DataVariant((uint64_t)12850912328012))),
        make_tuple(make_shared<ResourceDescriptor>(
                       1, "Test", OperationsType::READ_AND_WRITE, false, true,
                       DataType::TIME, "", ""),
                   DataFormat(DataVariant((uint64_t)12850912328012))),
        make_tuple(make_shared<ResourceDescriptor>(1, "Test",
                                                   OperationsType::EXECUTE,
                                                   false, true, DataType::TIME,
                                                   "", ""),
                   DataFormat(DataVariant((uint64_t)12850912328012)))),
    GenerateTestName());