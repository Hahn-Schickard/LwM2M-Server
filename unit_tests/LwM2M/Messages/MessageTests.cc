// Registration_Messages
#include "Deregister.hpp"
#include "Register.hpp"
#include "Update.hpp"
// Device_Management_Messages
#include "Create.hpp"
#include "Delete.hpp"
#include "Discover.hpp"
#include "Execute.hpp"
#include "Read.hpp"
#include "ReadComposite.hpp"
#include "Write.hpp"
#include "WriteAttributes.hpp"
#include "WriteComposite.hpp"
// Information_Reporting_Messages
#include "CancelObservation.hpp"
#include "CancelObserveComposite.hpp"
#include "Notify.hpp"
#include "Observe.hpp"
#include "ObserveComposite.hpp"
#include "Send.hpp"

#include "gtest/gtest.h"

#include <cassert>
#include <memory>
#include <type_traits>
#include <vector>

using namespace std;
using namespace LwM2M;

struct MessageTestExpectations {
  const EndpointPtr endpoint_;
  const MessageType message_type_;
  const InterfaceType interface_;
  const bool response_;
  const bool incoming_;
  const bool notification_;
};

using TestExpectationsPtr = shared_ptr<MessageTestExpectations>;

struct MessageTestParameter {
  MessagePtr tested_;
  TestExpectationsPtr expected_;

  MessageTestParameter(MessagePtr tested, TestExpectationsPtr expected)
      : tested_(tested), expected_(expected) {}
};

class MessageTest : public testing::TestWithParam<MessageTestParameter> {
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
  EXPECT_EQ(tested_->incoming_, expected_->incoming_);
}

TEST_P(MessageTest, returnsCorrectIsNotificationFlag) {
  EXPECT_EQ(tested_->notification_, expected_->notification_);
}

TEST_P(MessageTest, returnsCorrectInterfaceType) {
  EXPECT_EQ(tested_->interface_, expected_->interface_);
}

struct GenerateTestName {
  string operator()(
      const testing::TestParamInfo<MessageTestParameter> &parameter) const {
    return parameter.param.tested_->name();
  }
};

EndpointPtr tested_endpoint = make_shared<Endpoint>();

template <typename T>
MessageTestParameter
makeTestParameter(const MessageTestExpectations &valid_expectations) {
  assert((is_base_of<Message, T>::value));

  shared_ptr<T> message;
  try {
    message = make_shared<T>(tested_endpoint);
  } catch (exception &ex) {
    cerr << "Caught an unhandled exception while building "
         << toString(valid_expectations.message_type_)
         << " message test. Exception was: " << ex.what() << endl;
    throw ex;
  }
  auto expectations = make_shared<MessageTestExpectations>(valid_expectations);

  return MessageTestParameter(move(message), move(expectations));
}

INSTANTIATE_TEST_SUITE_P(
    BaseFieldTests, MessageTest,
    testing::Values(
        // Registration Messages
        makeTestParameter<RegisterRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::REGISTER,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = false,
            .incoming_ = true,
            .notification_ = false}),
        makeTestParameter<RegisterResponse>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::REGISTER,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = true,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<UpdateRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::UPDATE,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = false,
            .incoming_ = true,
            .notification_ = false}),
        makeTestParameter<UpdateResponse>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::UPDATE,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = true,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<DeregisterRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::DEREGISTER,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = false,
            .incoming_ = true,
            .notification_ = false}),
        makeTestParameter<DeregisterResponse>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::DEREGISTER,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = true,
            .incoming_ = false,
            .notification_ = false}),
        // Device management Messages
        makeTestParameter<CreateRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::CREATE,
            .interface_ = InterfaceType::DEVICE_MANAGEMENT,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<DeleteRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::DELETE,
            .interface_ = InterfaceType::DEVICE_MANAGEMENT,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<DiscoverRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::DISCOVER,
            .interface_ = InterfaceType::DEVICE_MANAGEMENT,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<ExecuteRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::EXECUTE,
            .interface_ = InterfaceType::DEVICE_MANAGEMENT,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<ReadRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::READ,
            .interface_ = InterfaceType::DEVICE_MANAGEMENT,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<ReadCompositeRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::READ_COMPOSITE,
            .interface_ = InterfaceType::DEVICE_MANAGEMENT,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<WriteRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::WRITE,
            .interface_ = InterfaceType::DEVICE_MANAGEMENT,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<WriteAttributesRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::WRITE_ATTRIBUTES,
            .interface_ = InterfaceType::DEVICE_MANAGEMENT,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<WriteCompositeRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::WRITE_COMPOSITE,
            .interface_ = InterfaceType::DEVICE_MANAGEMENT,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        // Information Reporting Messages
        makeTestParameter<ObserveRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::OBSERVE,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<ObserveCompositeRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::OBSERVE_COMPOSITE,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<ValueUpdated>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::NOTIFY,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incoming_ = true,
            .notification_ = true}),
        makeTestParameter<SendRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::SEND,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incoming_ = true,
            .notification_ = false}),
        makeTestParameter<SendResponse>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::SEND,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = true,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<CancelObservationRequest>(MessageTestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::CANCEL_OBSERVATION,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incoming_ = false,
            .notification_ = false}),
        makeTestParameter<CancelObserveCompositeRequest>(
            MessageTestExpectations{
                .endpoint_ = tested_endpoint,
                .message_type_ = MessageType::CANCEL_OBSERVATION_COMPOSITE,
                .interface_ = InterfaceType::INFORMATION_REPORTING,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false})),
    GenerateTestName());