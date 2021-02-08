#include "ResourceTestSharedInfo.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace LwM2M;

class SignedIntResourceTest
    : public testing::TestWithParam<ResourceTestParameter<int64_t>> {
protected:
  void SetUp() override {
    tested_ = GetParam().tested_;
    expected_ = GetParam().expected_;
  }

  ResourcePtr<int64_t> tested_;
  ResourceExpectationsPtr expected_;
  int response_delay_ms = 1;
};

TEST_P(SignedIntResourceTest, canReadValue) {
  if (expected_->descriptor_->operations_ == OperationsType::READ ||
      expected_->descriptor_->operations_ == OperationsType::READ_AND_WRITE) {
    EXPECT_NO_THROW({
      auto result = tested_->read();
      auto expected = expected_->result_;

      auto delay_executed =
          async(std::launch::async,
                RespondWithDelay<DataFormat>(expected_->requester_,
                                             response_delay_ms, expected));

      auto status =
          delay_executed.wait_for(chrono::milliseconds(response_delay_ms + 1));
      if (status == std::future_status::timeout) {
        FAIL() << "Response was not sent in time" << endl;
      } else {
        EXPECT_EQ(result.get(), expected.get<int64_t>());
      }
    });
  } else {
    EXPECT_THROW({ tested_->read(); }, UnsupportedMethod);
  }
}

TEST_P(SignedIntResourceTest, canWriteValue) {
  if (expected_->descriptor_->operations_ == OperationsType::WRITE ||
      expected_->descriptor_->operations_ == OperationsType::READ_AND_WRITE) {
    EXPECT_NO_THROW({
      auto result = tested_->write(true);
      auto expected = true;

      auto delay_executed =
          async(std::launch::async,
                RespondWithDelay<bool>(expected_->requester_, response_delay_ms,
                                       expected));

      auto status =
          delay_executed.wait_for(chrono::milliseconds(response_delay_ms + 1));
      if (status == std::future_status::timeout) {
        FAIL() << "Response was not sent in time" << endl;
      } else {
        EXPECT_TRUE(result.get());
      }
    });
  } else {
    EXPECT_THROW({ tested_->write(DataVariant()); }, UnsupportedMethod);
  }
}

TEST_P(SignedIntResourceTest, canExecuteAction) {
  if (expected_->descriptor_->operations_ == OperationsType::EXECUTE) {
    EXPECT_NO_THROW({
      auto result = tested_->execute();
      auto expected = true;

      auto delay_executed =
          async(std::launch::async,
                RespondWithDelay<bool>(expected_->requester_, response_delay_ms,
                                       expected));

      auto status =
          delay_executed.wait_for(chrono::milliseconds(response_delay_ms + 1));
      if (status == std::future_status::timeout) {
        FAIL() << "Response was not sent in time" << endl;
      } else {
        EXPECT_TRUE(result.get());
      }
    });
  } else {
    EXPECT_THROW({ tested_->execute(); }, UnsupportedMethod);
  }
}

struct GenerateTestName {
  std::string operator()(
      const testing::TestParamInfo<ResourceTestParameter<int64_t>> &parameter)
      const {
    auto name = parameter.param.tested_->getDescriptor()->name_ +
                toString(parameter.param.tested_->getDescriptor()->data_type_) +
                toString(parameter.param.tested_->getDescriptor()->operations_);
    name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());
    return name;
  }
};

const ResourceExpectations &SignedIntReadable{
    .requester_ = test_requester,
    .endpoint_ = tested_endpoint,
    .parent_ = ObjectInstanceID(0, 0),
    .descriptor_ =
        make_shared<ResourceDescriptor>(1, "Test", OperationsType::READ, false,
                                        true, DataType::SIGNED_INTEGER, "", ""),
    .result_ =
        DataFormat(DataVariant((int64_t)-100), DataType::SIGNED_INTEGER)};

INSTANTIATE_TEST_SUITE_P(
    SignedIntResourceTests, SignedIntResourceTest,
    testing::Values(makeTestParameter<int64_t>(SignedIntReadable)),
    GenerateTestName());