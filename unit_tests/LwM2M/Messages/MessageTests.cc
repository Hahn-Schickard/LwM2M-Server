// Registration_Messages
#include "Deregister.hpp"
#include "Register.hpp"
#include "Update.hpp"
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

EndpointPtr tested_endpoint = make_shared<Endpoint>();

template <typename T>
TestParameter makeTestParameter(const TestExpectations &valid_expectations) {
  assert((is_base_of<Message, T>::value));

  shared_ptr<T> message;
  try {
    message = make_shared<T>(tested_endpoint);
  } catch (exception &ex) {
    cerr << "Caught an unhandeled exception while building "
         << toString(valid_expectations.message_type_)
         << " message test. Exception was: " << ex.what() << endl;
    throw ex;
  }
  auto expectations = make_shared<TestExpectations>(valid_expectations);

  return TestParameter(move(message), move(expectations));
}

INSTANTIATE_TEST_SUITE_P(
    BaseFieldTests, MessageTest,
    testing::Values(
        // Registration Messages
        makeTestParameter<RegisterRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::REGISTER,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = false,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<RegisterResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::REGISTER,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = true,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<UpdateRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::UPDATE,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = false,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<UpdateResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::UPDATE,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = true,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<DeregisterRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::DEREGISTER,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = false,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<DeregisterResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::DEREGISTER,
            .interface_ = InterfaceType::REGISTRATION,
            .response_ = true,
            .incomming_ = false,
            .notification_ = false}),
        // Device managment Messages
        makeTestParameter<CreateRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::CREATE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<CreateResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::CREATE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<DeleteRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::DELETE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<DeleteResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::DELETE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<DiscoverRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::DISCOVER,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<DiscoverResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::DISCOVER,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<ExecuteRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::EXECUTE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<ExecuteResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::EXECUTE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<ReadRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::READ,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<ReadResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::READ,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<ReadComopositeRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::READ_COMPOSITE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<ReadComopositeResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::READ_COMPOSITE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<WriteRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::WRITE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<WriteResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::WRITE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<WriteAttributesRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::WRITE_ATTRIBUTES,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<WriteAttributesResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::WRITE_ATTRIBUTES,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<WriteComopositeRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::WRITE_COMPOSITE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<WriteComopositeResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::WRITE_COMPOSITE,
            .interface_ = InterfaceType::DEVICE_MANAGMENT,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        // Information Reporting Messages
        makeTestParameter<ObserveRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::OBSERVE,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<ObservResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::OBSERVE,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<ObserveCompositeRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::OBSERVE_COMPOSITE,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<ObserveCompositeResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::OBSERVE_COMPOSITE,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<ValueUpdated>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::NOTIFY,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incomming_ = true,
            .notification_ = true}),
        makeTestParameter<SendRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::SEND,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<SendResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::SEND,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = true,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<CancelObservationRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::CANCEL_OBSERVATION,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<CancelObservationResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::CANCEL_OBSERVATION,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false}),
        makeTestParameter<CancelObserveCompositeRequest>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::CANCEL_OBSERVATION_COMPOSITE,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = false,
            .incomming_ = false,
            .notification_ = false}),
        makeTestParameter<CancelObserveCompositeResponse>(TestExpectations{
            .endpoint_ = tested_endpoint,
            .message_type_ = MessageType::CANCEL_OBSERVATION_COMPOSITE,
            .interface_ = InterfaceType::INFORMATION_REPORTING,
            .response_ = true,
            .incomming_ = true,
            .notification_ = false})),
    GenerateTestName());