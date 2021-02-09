#include "ResourceTestSharedInfo.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace LwM2M;

class OpaqueResourceTest
    : public testing::TestWithParam<ResourceTestParameter<vector<uint8_t>>> {
protected:
  void SetUp() override {
    tested_ = GetParam().tested_;
    expected_ = GetParam().expected_;
  }

  ResourcePtr<vector<uint8_t>> tested_;
  ResourceExpectationsPtr expected_;
  int response_delay_ms = 1;
};

TEST_P(OpaqueResourceTest, canReadValue) {
  if (expected_->descriptor_->operations_ == OperationsType::READ ||
      expected_->descriptor_->operations_ == OperationsType::READ_AND_WRITE) {
    EXPECT_NO_THROW({
      auto result = tested_->read();
      auto expected = expected_->result_;

      async(launch::async,
            RespondWithDelay<DataFormat>(expected_->requester_,
                                         response_delay_ms, expected));
      EXPECT_EQ(result.get(), expected.get<vector<uint8_t>>());
    });
  } else {
    EXPECT_THROW({ tested_->read(); }, UnsupportedMethod);
  }
}

TEST_P(OpaqueResourceTest, canWriteValue) {
  if (expected_->descriptor_->operations_ == OperationsType::WRITE ||
      expected_->descriptor_->operations_ == OperationsType::READ_AND_WRITE) {
    EXPECT_NO_THROW({
      auto result = tested_->write(true);
      auto expected = true;

      async(std::launch::async,
            RespondWithDelay<bool>(expected_->requester_, response_delay_ms,
                                   expected));
      EXPECT_TRUE(result.get());
    });
  } else {
    EXPECT_THROW({ tested_->write(DataVariant()); }, UnsupportedMethod);
  }
}

TEST_P(OpaqueResourceTest, canExecuteAction) {
  if (expected_->descriptor_->operations_ == OperationsType::EXECUTE) {
    EXPECT_NO_THROW({
      auto result = tested_->execute();
      auto expected = true;

      async(std::launch::async,
            RespondWithDelay<bool>(expected_->requester_, response_delay_ms,
                                   expected));
      EXPECT_TRUE(result.get());
    });
  } else {
    EXPECT_THROW({ tested_->execute(); }, UnsupportedMethod);
  }
}

struct GenerateTestName {
  std::string operator()(
      const testing::TestParamInfo<ResourceTestParameter<vector<uint8_t>>>
          &parameter) const {
    auto name = parameter.param.tested_->getDescriptor()->name_ +
                toString(parameter.param.tested_->getDescriptor()->data_type_) +
                toString(parameter.param.tested_->getDescriptor()->operations_);
    name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());
    return name;
  }
};

const ResourceExpectations &OpaqueReadable{
    .requester_ = test_requester,
    .endpoint_ = tested_endpoint,
    .parent_ = ObjectInstanceID(0, 0),
    .descriptor_ = make_shared<ResourceDescriptor>(
        1, "Test", OperationsType::READ, false, true, DataType::OPAQUE, "", ""),
    .result_ = DataFormat(DataVariant(vector<uint8_t>{1, 2, 3, 4, 5}),
                          DataType::OPAQUE)};

const ResourceExpectations &OpaqueWritable{
    .requester_ = test_requester,
    .endpoint_ = tested_endpoint,
    .parent_ = ObjectInstanceID(0, 0),
    .descriptor_ =
        make_shared<ResourceDescriptor>(1, "Test", OperationsType::WRITE, false,
                                        true, DataType::OPAQUE, "", ""),
    .result_ =
        DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}), DataType::OPAQUE)};

const ResourceExpectations &OpaqueReadAndWritable{
    .requester_ = test_requester,
    .endpoint_ = tested_endpoint,
    .parent_ = ObjectInstanceID(0, 0),
    .descriptor_ = make_shared<ResourceDescriptor>(
        1, "Test", OperationsType::READ_AND_WRITE, false, true,
        DataType::OPAQUE, "", ""),
    .result_ =
        DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}), DataType::OPAQUE)};

const ResourceExpectations &OpaqueExecutable{
    .requester_ = test_requester,
    .endpoint_ = tested_endpoint,
    .parent_ = ObjectInstanceID(0, 0),
    .descriptor_ =
        make_shared<ResourceDescriptor>(1, "Test", OperationsType::EXECUTE,
                                        false, true, DataType::OPAQUE, "", ""),
    .result_ =
        DataFormat(DataVariant(vector<uint8_t>{2, 3, 6}), DataType::OPAQUE)};

INSTANTIATE_TEST_SUITE_P(
    OpaqueResourceTests, OpaqueResourceTest,
    testing::Values(makeTestParameter<vector<uint8_t>>(OpaqueReadable),
                    makeTestParameter<vector<uint8_t>>(OpaqueWritable),
                    makeTestParameter<vector<uint8_t>>(OpaqueReadAndWritable),
                    makeTestParameter<vector<uint8_t>>(OpaqueExecutable)),
    GenerateTestName());