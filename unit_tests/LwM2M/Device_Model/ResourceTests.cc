#include "Executable.hpp"
#include "ReadAndWritable.hpp"
#include "Readable.hpp"
#include "TestRequester.hpp"
#include "Variant_Visitor.hpp"
#include "Writable.hpp"

#include "gtest/gtest.h"

#include <chrono>
#include <memory>
#include <thread>
#include <variant>

using namespace std;
using namespace LwM2M;

using ResourceVariant =
    variant<ResourcePtr<bool>, ResourcePtr<int64_t>, ResourcePtr<uint64_t>,
            ResourcePtr<double>, ResourcePtr<string>, ResourcePtr<ObjectLink>,
            ResourcePtr<vector<uint8_t>>>;

struct ResourceExpectations {
  TestRequesterPtr requester_;
  EndpointPtr endpoint_;
  ResourceDescriptorPtr descriptor_;
  DataFormat result_;
};

using ResourceExpectationsPtr = shared_ptr<ResourceExpectations>;

struct ResourceTestParameter {
  ResourceVariant tested_;
  ResourceExpectationsPtr expected_;

  ResourceTestParameter(ResourceVariant tested,
                        ResourceExpectationsPtr expected)
      : tested_(tested), expected_(expected) {}
};

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

class ResourceTest : public testing::TestWithParam<ResourceTestParameter> {
protected:
  void SetUp() override {
    tested_ = GetParam().tested_;
    expected_ = GetParam().expected_;
  }

  template <typename T> void readResource(ResourcePtr<T> resource) {
    try {
      auto result = resource->read();
      auto expected = expected_->result_;

      async(std::launch::async,
            RespondWithDelay<DataFormat>(expected_->requester_,
                                         response_delay_ms, expected));
      EXPECT_EQ(result.get(), expected.get<T>());
    } catch (exception &ex) {
      FAIL() << "Caught exception: " << ex.what() << endl;
    }
  }

  template <typename T> void writeResource(ResourcePtr<T> resource) {
    try {
      auto result = resource->write(expected_->result_.data_);

      async(std::launch::async,
            RespondWithDelay<bool>(expected_->requester_, response_delay_ms,
                                   true));
      EXPECT_TRUE(result.get());
    } catch (exception &ex) {
      FAIL() << "Caught exception: " << ex.what() << endl;
    }
  }

  template <typename T> void executeResource(ResourcePtr<T> resource) {
    try {
      auto result = resource->execute();

      async(std::launch::async,
            RespondWithDelay<bool>(expected_->requester_, response_delay_ms,
                                   true));
      EXPECT_TRUE(result.get());
    } catch (exception &ex) {
      FAIL() << "Caught exception: " << ex.what() << endl;
    }
  }

  ResourceVariant tested_;
  ResourceExpectationsPtr expected_;
  int response_delay_ms = 1;
};

TEST_P(ResourceTest, isValidResourceVariant) {
  ASSERT_FALSE(tested_.valueless_by_exception());
}

TEST_P(ResourceTest, canReadValue) {
  if (expected_->descriptor_->operations_ == OperationsType::READ ||
      expected_->descriptor_->operations_ == OperationsType::READ_AND_WRITE) {
    match(
        tested_,
        [&](ResourcePtr<bool> resource) { readResource<bool>(resource); },
        [&](ResourcePtr<int64_t> resource) { readResource<int64_t>(resource); },
        [&](ResourcePtr<uint64_t> resource) {
          readResource<uint64_t>(resource);
        },
        [&](ResourcePtr<double> resource) { readResource<double>(resource); },
        [&](ResourcePtr<string> resource) { readResource<string>(resource); },
        [&](ResourcePtr<ObjectLink> resource) {
          readResource<ObjectLink>(resource);
        },
        [&](ResourcePtr<vector<uint8_t>> resource) {
          readResource<vector<uint8_t>>(resource);
        });
  } else {
    match(tested_,
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

TEST_P(ResourceTest, canWriteValue) {
  if (expected_->descriptor_->operations_ == OperationsType::WRITE ||
      expected_->descriptor_->operations_ == OperationsType::READ_AND_WRITE) {
    match(
        tested_,
        [&](ResourcePtr<bool> resource) { writeResource<bool>(resource); },
        [&](ResourcePtr<int64_t> resource) {
          writeResource<int64_t>(resource);
        },
        [&](ResourcePtr<uint64_t> resource) {
          writeResource<uint64_t>(resource);
        },
        [&](ResourcePtr<double> resource) { writeResource<double>(resource); },
        [&](ResourcePtr<string> resource) { writeResource<string>(resource); },
        [&](ResourcePtr<ObjectLink> resource) {
          writeResource<ObjectLink>(resource);
        },
        [&](ResourcePtr<vector<uint8_t>> resource) {
          writeResource<vector<uint8_t>>(resource);
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

TEST_P(ResourceTest, canExecuteAction) {
  if (expected_->descriptor_->operations_ == OperationsType::EXECUTE) {
    match(tested_,
          [&](ResourcePtr<bool> resource) { executeResource<bool>(resource); },
          [&](ResourcePtr<int64_t> resource) {
            executeResource<int64_t>(resource);
          },
          [&](ResourcePtr<uint64_t> resource) {
            executeResource<uint64_t>(resource);
          },
          [&](ResourcePtr<double> resource) {
            executeResource<double>(resource);
          },
          [&](ResourcePtr<string> resource) {
            executeResource<string>(resource);
          },
          [&](ResourcePtr<ObjectLink> resource) {
            executeResource<ObjectLink>(resource);
          },
          [&](ResourcePtr<vector<uint8_t>> resource) {
            executeResource<vector<uint8_t>>(resource);
          });
  } else {
    match(tested_,
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
    auto name = parameter.param.expected_->descriptor_->name_ +
                toString(parameter.param.expected_->descriptor_->data_type_) +
                toString(parameter.param.expected_->descriptor_->operations_);
    name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
    name.erase(
        remove_if(name.begin(), name.end(),
                  [](unsigned char x) { return x == '_' ? true : false; }),
        name.end());
    return name;
  }
};

template <typename T>
ResourceVariant makeTestResource(TestRequesterPtr requester,
                                 EndpointPtr endpoint, ObjectInstanceID parent,
                                 ResourceDescriptorPtr descriptor) {
  switch (descriptor->operations_) {
  case OperationsType::READ: {
    return ResourceVariant(
        make_shared<Readable<T>>(requester, endpoint, parent, descriptor));
  }
  case OperationsType::READ_AND_WRITE: {
    return ResourceVariant(make_shared<ReadAndWritable<T>>(requester, endpoint,
                                                           parent, descriptor));
  }
  case OperationsType::WRITE: {
    return ResourceVariant(
        make_shared<Writable<T>>(requester, endpoint, parent, descriptor));
  }
  case OperationsType::EXECUTE: {
    return ResourceVariant(
        make_shared<Executable<T>>(requester, endpoint, parent, descriptor));
  }
  default: {
    throw invalid_argument("Resource operation type is invalid. Unit tests "
                           "only support valid resources!");
  }
  }
}

ResourceTestParameter makeTestParameter(ResourceDescriptorPtr descriptor,
                                        DataFormat result) {
  auto requester = make_shared<TestRequester>();
  auto endpoint = make_shared<Endpoint>("0.0.0.0", 86544);

  ResourceVariant resource;
  switch (descriptor->data_type_) {
  case DataType::BOOLEAN: {
    resource = makeTestResource<bool>(requester, endpoint,
                                      ObjectInstanceID(0, 0), descriptor);
    break;
  }
  case DataType::SIGNED_INTEGER: {
    resource = makeTestResource<int64_t>(requester, endpoint,
                                         ObjectInstanceID(0, 0), descriptor);
    break;
  }
  case DataType::UNSIGNED_INTEGER:
  case DataType::TIME: {
    resource = makeTestResource<uint64_t>(requester, endpoint,
                                          ObjectInstanceID(0, 0), descriptor);
    break;
  }
  case DataType::FLOAT: {
    resource = makeTestResource<double>(requester, endpoint,
                                        ObjectInstanceID(0, 0), descriptor);
    break;
  }
  case DataType::STRING: {
    resource = makeTestResource<string>(requester, endpoint,
                                        ObjectInstanceID(0, 0), descriptor);
    break;
  }
  case DataType::OPAQUE: {
    resource = makeTestResource<vector<uint8_t>>(
        requester, endpoint, ObjectInstanceID(0, 0), descriptor);
    break;
  }
  case DataType::OBJECT_LINK: {
    resource = makeTestResource<ObjectLink>(requester, endpoint,
                                            ObjectInstanceID(0, 0), descriptor);
    break;
  }
  default: {
    throw invalid_argument("Resource data type is invalid. Unit tests "
                           "only support valid resources!");
  }
  }
  auto expectations = make_shared<ResourceExpectations>(
      ResourceExpectations{.requester_ = requester,
                           .endpoint_ = endpoint,
                           .descriptor_ = descriptor,
                           .result_ = result});

  return ResourceTestParameter(resource, expectations);
}

INSTANTIATE_TEST_SUITE_P(
    ResourceTests, ResourceTest,
    testing::Values(
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ, false, true,
                              DataType::BOOLEAN, "", ""),
                          DataFormat(DataVariant((bool)true))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::WRITE, false, true,
                              DataType::BOOLEAN, "", ""),
                          DataFormat(DataVariant((bool)true))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ_AND_WRITE, false,
                              true, DataType::BOOLEAN, "", ""),
                          DataFormat(DataVariant((bool)true))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::EXECUTE, false, true,
                              DataType::BOOLEAN, "", ""),
                          DataFormat(DataVariant((bool)true))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ, false, true,
                              DataType::FLOAT, "", ""),
                          DataFormat(DataVariant((double)52.4))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::WRITE, false, true,
                              DataType::FLOAT, "", ""),
                          DataFormat(DataVariant((double)20.2))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ_AND_WRITE, false,
                              true, DataType::FLOAT, "", ""),
                          DataFormat(DataVariant((double)20.2))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::EXECUTE, false, true,
                              DataType::FLOAT, "", ""),
                          DataFormat(DataVariant((double)20.2))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ, false, true,
                              DataType::OBJECT_LINK, "", ""),
                          DataFormat(DataVariant(ObjectLink(0, 0)))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::WRITE, false, true,
                              DataType::OBJECT_LINK, "", ""),
                          DataFormat(DataVariant(ObjectLink(0, 0)))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ_AND_WRITE, false,
                              true, DataType::OBJECT_LINK, "", ""),
                          DataFormat(DataVariant(ObjectLink(0, 0)))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::EXECUTE, false, true,
                              DataType::OBJECT_LINK, "", ""),
                          DataFormat(DataVariant(ObjectLink(0, 0)))),
        makeTestParameter(
            make_shared<ResourceDescriptor>(1, "Test", OperationsType::READ,
                                            false, true, DataType::OPAQUE, "",
                                            ""),
            DataFormat(DataVariant(vector<uint8_t>{1, 2, 3, 4, 5}))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::WRITE, false, true,
                              DataType::OPAQUE, "", ""),
                          DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ_AND_WRITE, false,
                              true, DataType::OPAQUE, "", ""),
                          DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::EXECUTE, false, true,
                              DataType::OPAQUE, "", ""),
                          DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ, false, true,
                              DataType::SIGNED_INTEGER, "", ""),
                          DataFormat(DataVariant((int64_t)-100))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::WRITE, false, true,
                              DataType::SIGNED_INTEGER, "", ""),
                          DataFormat(DataVariant((int64_t)-100))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ_AND_WRITE, false,
                              true, DataType::SIGNED_INTEGER, "", ""),
                          DataFormat(DataVariant((int64_t)-100))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::EXECUTE, false, true,
                              DataType::SIGNED_INTEGER, "", ""),
                          DataFormat(DataVariant((int64_t)-100))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ, false, true,
                              DataType::STRING, "", ""),
                          DataFormat(DataVariant((string) "Hello"))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::WRITE, false, true,
                              DataType::STRING, "", ""),
                          DataFormat(DataVariant(string("hello")))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ_AND_WRITE, false,
                              true, DataType::STRING, "", ""),
                          DataFormat(DataVariant(string("hello")))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::EXECUTE, false, true,
                              DataType::STRING, "", ""),
                          DataFormat(DataVariant(string("hello")))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ, false, true,
                              DataType::UNSIGNED_INTEGER, "", ""),
                          DataFormat(DataVariant((uint64_t)26))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::WRITE, false, true,
                              DataType::UNSIGNED_INTEGER, "", ""),
                          DataFormat(DataVariant((uint64_t)26))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ_AND_WRITE, false,
                              true, DataType::UNSIGNED_INTEGER, "", ""),
                          DataFormat(DataVariant((uint64_t)26))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::EXECUTE, false, true,
                              DataType::UNSIGNED_INTEGER, "", ""),
                          DataFormat(DataVariant((uint64_t)26))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ, false, true,
                              DataType::TIME, "", ""),
                          DataFormat(DataVariant((uint64_t)12850912328012))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::WRITE, false, true,
                              DataType::TIME, "", ""),
                          DataFormat(DataVariant((uint64_t)12850912328012))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::READ_AND_WRITE, false,
                              true, DataType::TIME, "", ""),
                          DataFormat(DataVariant((uint64_t)12850912328012))),
        makeTestParameter(make_shared<ResourceDescriptor>(
                              1, "Test", OperationsType::EXECUTE, false, true,
                              DataType::TIME, "", ""),
                          DataFormat(DataVariant((uint64_t)12850912328012)))),
    GenerateTestName());