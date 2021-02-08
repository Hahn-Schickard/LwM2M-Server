#include "ResourceTestSharedInfo.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace LwM2M;

class StringResourceTest
    : public testing::TestWithParam<ResourceTestParameter<string>> {
protected:
  void SetUp() override {
    tested_ = GetParam().tested_;
    expected_ = GetParam().expected_;
  }

  ResourcePtr<string> tested_;
  ResourceExpectationsPtr expected_;
  int response_delay_ms = 1;
};

TEST_P(StringResourceTest, canReadValue) {
  if (expected_->descriptor_->operations_ == OperationsType::READ ||
      expected_->descriptor_->operations_ == OperationsType::READ_AND_WRITE) {
    EXPECT_NO_THROW({
      auto result = tested_->read();
      auto expected = expected_->result_;

      async(std::launch::async,
            RespondWithDelay<DataFormat>(expected_->requester_,
                                         response_delay_ms, expected));
      EXPECT_EQ(result.get(), expected.get<string>());
    });
  } else {
    EXPECT_THROW({ tested_->read(); }, UnsupportedMethod);
  }
}

TEST_P(StringResourceTest, canWriteValue) {
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

TEST_P(StringResourceTest, canExecuteAction) {
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
      const testing::TestParamInfo<ResourceTestParameter<string>> &parameter)
      const {
    auto name = parameter.param.tested_->getDescriptor()->name_ +
                toString(parameter.param.tested_->getDescriptor()->data_type_) +
                toString(parameter.param.tested_->getDescriptor()->operations_);
    name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());
    return name;
  }
};

const ResourceExpectations &StringReadable{
    .requester_ = test_requester,
    .endpoint_ = tested_endpoint,
    .parent_ = ObjectInstanceID(0, 0),
    .descriptor_ = make_shared<ResourceDescriptor>(
        1, "Test", OperationsType::READ, false, true, DataType::STRING, "", ""),
    .result_ = DataFormat(DataVariant((string) "Hello"), DataType::STRING)};

INSTANTIATE_TEST_SUITE_P(
    StringResourceTest, StringResourceTest,
    testing::Values(makeTestParameter<string>(StringReadable)),
    GenerateTestName());