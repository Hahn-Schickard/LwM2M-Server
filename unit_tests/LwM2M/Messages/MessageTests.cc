// Registration_Messages
// #include "Deregister.hpp"
// #include "Register.hpp"
// #include "Update.hpp"
// Device_Managment_Messages
#include "Create.hpp"
#include "Delete.hpp"
#include "Discover.hpp"
#include "Execute.hpp"
#include "Read.hpp"
#include "ReadComoposite.hpp"
#include "Write.hpp"
#include "WriteAttributes.hpp"
#include "WriteComoposite.hpp"

#include "gtest/gtest.h"

#include <memory>
#include <vector>

using namespace std;
using namespace LwM2M;

struct TestExpectations {
  const EndpointPtr endpoint_;
  const MessageType message_type_;
  const InterfaceType interface_;
  const bool response_;
  const bool incomming_;
  const bool notification_;
};

using TestExpectationsPtr = shared_ptr<TestExpectations>;

struct TestParameter {
  MessagePtr tested_;
  TestExpectationsPtr expected_;

  TestParameter(MessagePtr tested, TestExpectationsPtr expected)
      : tested_(tested), expected_(expected) {}
};

class MessageTest : public testing::TestWithParam<TestParameter> {
public:
  void SetUp() override {
    tested_ = GetParam().tested_;
    expected_ = GetParam().expected_;
  }

  MessagePtr tested_;
  TestExpectationsPtr expected_;
};

TEST_P(MessageTest, returnsCorrectMessageType) {
  EXPECT_EQ(tested_->message_type_, expected_->message_type_);
}

TEST_P(MessageTest, returnsCorrectIsResponseFlag) {
  EXPECT_EQ(tested_->response_, expected_->response_);
}

TEST_P(MessageTest, returnsCorrectIsIncommingFlag) {
  EXPECT_EQ(tested_->incomming_, expected_->incomming_);
}

TEST_P(MessageTest, returnsCorrectIsNotificationFlag) {
  EXPECT_EQ(tested_->notification_, expected_->notification_);
}

TEST_P(MessageTest, returnsCorrectInterfaceType) {
  EXPECT_EQ(tested_->interface_, expected_->interface_);
}

struct GenerateTestName {
  string
  operator()(const testing::TestParamInfo<TestParameter> &parameter) const {
    return parameter.param.tested_->name();
  }
};

INSTANTIATE_TEST_SUITE_P(
    BaseFieldTests, MessageTest,
    testing::Values(
        TestParameter(make_shared<CreateRequest>(make_shared<Endpoint>(),
                                                 ObjectID(3303),
                                                 DataFormatPtr()),
                      make_shared<TestExpectations>(TestExpectations{
                          .endpoint_ = make_shared<Endpoint>(),
                          .message_type_ = MessageType::CREATE,
                          .interface_ = InterfaceType::DEVICE_MANAGMENT,
                          .response_ = false,
                          .incomming_ = false,
                          .notification_ = false})),
        TestParameter(make_shared<DeleteRequest>(make_shared<Endpoint>(),
                                                 ObjectInstanceID(3303, 1)),
                      make_shared<TestExpectations>(TestExpectations{
                          .endpoint_ = make_shared<Endpoint>(),
                          .message_type_ = MessageType::DELETE,
                          .interface_ = InterfaceType::DEVICE_MANAGMENT,
                          .response_ = false,
                          .incomming_ = false,
                          .notification_ = false})),
        TestParameter(make_shared<DiscoverRequest>(make_shared<Endpoint>(),
                                                   ObjectInstanceID(3303, 1)),
                      make_shared<TestExpectations>(TestExpectations{
                          .endpoint_ = make_shared<Endpoint>(),
                          .message_type_ = MessageType::DISCOVER,
                          .interface_ = InterfaceType::DEVICE_MANAGMENT,
                          .response_ = false,
                          .incomming_ = false,
                          .notification_ = false})),
        TestParameter(make_shared<ExecuteRequest>(make_shared<Endpoint>(),
                                                  ResourceID(3303, 0, 1)),
                      make_shared<TestExpectations>(TestExpectations{
                          .endpoint_ = make_shared<Endpoint>(),
                          .message_type_ = MessageType::EXECUTE,
                          .interface_ = InterfaceType::DEVICE_MANAGMENT,
                          .response_ = false,
                          .incomming_ = false,
                          .notification_ = false})),
        TestParameter(make_shared<ReadRequest>(make_shared<Endpoint>(),
                                               ResourceID(3303, 0, 1)),
                      make_shared<TestExpectations>(TestExpectations{
                          .endpoint_ = make_shared<Endpoint>(),
                          .message_type_ = MessageType::READ,
                          .interface_ = InterfaceType::DEVICE_MANAGMENT,
                          .response_ = false,
                          .incomming_ = false,
                          .notification_ = false})),
        TestParameter(make_shared<ReadComopositeRequest>(
                          make_shared<Endpoint>(), ResourceID(3303, 0, 1)),
                      make_shared<TestExpectations>(TestExpectations{
                          .endpoint_ = make_shared<Endpoint>(),
                          .message_type_ = MessageType::READ_COMPOSITE,
                          .interface_ = InterfaceType::DEVICE_MANAGMENT,
                          .response_ = false,
                          .incomming_ = false,
                          .notification_ = false})),
        TestParameter(make_shared<WriteRequest>(make_shared<Endpoint>(),
                                                DataFormatPtr(),
                                                ResourceID(3303, 0, 1)),
                      make_shared<TestExpectations>(TestExpectations{
                          .endpoint_ = make_shared<Endpoint>(),
                          .message_type_ = MessageType::WRITE,
                          .interface_ = InterfaceType::DEVICE_MANAGMENT,
                          .response_ = false,
                          .incomming_ = false,
                          .notification_ = false})),
        TestParameter(make_shared<WriteAttributesRequest>(
                          make_shared<Endpoint>(), vector<TargetAttribute>()),
                      make_shared<TestExpectations>(TestExpectations{
                          .endpoint_ = make_shared<Endpoint>(),
                          .message_type_ = MessageType::WRITE_ATTRIBUTES,
                          .interface_ = InterfaceType::DEVICE_MANAGMENT,
                          .response_ = false,
                          .incomming_ = false,
                          .notification_ = false})),
        TestParameter(make_shared<WriteComopositeRequest>(
                          make_shared<Endpoint>(), vector<TargetContent>()),
                      make_shared<TestExpectations>(TestExpectations{
                          .endpoint_ = make_shared<Endpoint>(),
                          .message_type_ = MessageType::WRITE_COMPOSITE,
                          .interface_ = InterfaceType::DEVICE_MANAGMENT,
                          .response_ = false,
                          .incomming_ = false,
                          .notification_ = false}))),
    GenerateTestName());