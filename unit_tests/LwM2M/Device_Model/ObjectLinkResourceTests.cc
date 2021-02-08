#include "ResourceTestSharedInfo.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace LwM2M;

class ObjectLinkResourceTest
    : public testing::TestWithParam<ResourceTestParameter<ObjectLink>> {
protected:
  void SetUp() override {
    tested_ = GetParam().tested_;
    expected_ = GetParam().expected_;
  }

  ResourcePtr<ObjectLink> tested_;
  ResourceExpectationsPtr expected_;
  int response_delay_ms = 1;
};

TEST_P(ObjectLinkResourceTest, canReadValue) {
  if (expected_->descriptor_->operations_ == OperationsType::READ ||
      expected_->descriptor_->operations_ == OperationsType::READ_AND_WRITE) {
    EXPECT_NO_THROW({
      auto result = tested_->read();
      auto expected = expected_->result_;

      async(std::launch::async,
            RespondWithDelay<DataFormat>(expected_->requester_,
                                         response_delay_ms, expected));
      EXPECT_EQ(result.get(), expected.get<ObjectLink>());
    });
  } else {
    EXPECT_THROW({ tested_->read(); }, UnsupportedMethod);
  }
}

TEST_P(ObjectLinkResourceTest, canWriteValue) {
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

TEST_P(ObjectLinkResourceTest, canExecuteAction) {
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
  std::string
  operator()(const testing::TestParamInfo<ResourceTestParameter<ObjectLink>>
                 &parameter) const {
    auto name = parameter.param.tested_->getDescriptor()->name_ +
                toString(parameter.param.tested_->getDescriptor()->data_type_) +
                toString(parameter.param.tested_->getDescriptor()->operations_);
    name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());
    return name;
  }
};

const ResourceExpectations &ObjectLinkReadable{
    .requester_ = test_requester,
    .endpoint_ = tested_endpoint,
    .parent_ = ObjectInstanceID(0, 0),
    .descriptor_ =
        make_shared<ResourceDescriptor>(1, "Test", OperationsType::READ, false,
                                        true, DataType::OBJECT_LINK, "", ""),
    .result_ =
        DataFormat(DataVariant(ObjectLink(0, 0)), DataType::OBJECT_LINK)};

INSTANTIATE_TEST_SUITE_P(
    ObjectLinkResourceTests, ObjectLinkResourceTest,
    testing::Values(makeTestParameter<ObjectLink>(ObjectLinkReadable)),
    GenerateTestName());