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

// NOLINTBEGIN

struct MessageTestExpectations {
  const EndpointPtr endpoint_; // NOLINT
  const MessageType message_type_; // NOLINT
  const InterfaceType interface_; // NOLINT
  const bool response_; // NOLINT
  const bool incoming_; // NOLINT
  const bool notification_; // NOLINT
};

using TestExpectationsPtr = shared_ptr<MessageTestExpectations>;

struct MessageTestParameter {
  MessagePtr tested;
  TestExpectationsPtr expected;

  MessageTestParameter(
      MessagePtr tested_value, TestExpectationsPtr expected_value)
      : tested(move(tested_value)), expected(move(expected_value)) {}
};

class MessageTest : public testing::TestWithParam<MessageTestParameter> {
public:
  void SetUp() override {
    tested = GetParam().tested;
    expected = GetParam().expected;
  }

  MessagePtr tested;
  TestExpectationsPtr expected;
};

// NOLINTNEXTLINE
TEST_P(MessageTest, returnsCorrectMessageType) {
  EXPECT_EQ(tested->message_type_, expected->message_type_);
}

// NOLINTNEXTLINE
TEST_P(MessageTest, returnsCorrectIsResponseFlag) {
  EXPECT_EQ(tested->response_, expected->response_);
}

// NOLINTNEXTLINE
TEST_P(MessageTest, returnsCorrectIsIncommingFlag) {
  EXPECT_EQ(tested->incoming_, expected->incoming_);
}

// NOLINTNEXTLINE
TEST_P(MessageTest, returnsCorrectIsNotificationFlag) {
  EXPECT_EQ(tested->notification_, expected->notification_);
}

// NOLINTNEXTLINE
TEST_P(MessageTest, returnsCorrectInterfaceType) {
  EXPECT_EQ(tested->interface_, expected->interface_);
}

struct GenerateTestName {
  string operator()(
      const testing::TestParamInfo<MessageTestParameter>& parameter) const {
    return parameter.param.tested->name();
  }
};

EndpointPtr tested_endpoint = make_shared<Endpoint>("0.0.0.0", 86524); // NOLINT

template <typename T> shared_ptr<T> makeDefaultMessage(EndpointPtr endpoint) {
  return make_shared<T>(endpoint);
}

template <> RegisterResponsePtr makeDefaultMessage(EndpointPtr endpoint) {
  return make_shared<RegisterResponse>(endpoint, "");
}

template <> UpdateRequestPtr makeDefaultMessage(EndpointPtr endpoint) {
  return make_shared<UpdateRequest>(endpoint, "");
}

template <> UpdateResponsePtr makeDefaultMessage(EndpointPtr endpoint) {
  return make_shared<UpdateResponse>(endpoint, "");
}

template <> DeregisterRequestPtr makeDefaultMessage(EndpointPtr endpoint) {
  return make_shared<DeregisterRequest>(endpoint, "");
}

template <> DeregisterResponsePtr makeDefaultMessage(EndpointPtr endpoint) {
  return make_shared<DeregisterResponse>(endpoint, "");
}

template <typename T>
MessageTestParameter makeTestParameter(
    const MessageTestExpectations& valid_expectations) {
  assert((is_base_of<Message, T>::value));

  shared_ptr<T> message;
  try {
    message = makeDefaultMessage<T>(tested_endpoint);
  } catch (const exception& ex) {
    cerr << "Caught an unhandled exception while building "
         << toString(valid_expectations.message_type_)
         << " message test. Exception was: " << ex.what() << endl;
    throw ex;
  }
  auto expectations = make_shared<MessageTestExpectations>(valid_expectations);

  return MessageTestParameter(move(message), move(expectations));
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(BaseFieldTests, MessageTest,
    testing::Values(
        // Registration Messages
        makeTestParameter<RegisterRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Register,
                .interface_ = InterfaceType::Registration,
                .response_ = false,
                .incoming_ = true,
                .notification_ = false}),
        makeTestParameter<RegisterResponse>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Register,
                .interface_ = InterfaceType::Registration,
                .response_ = true,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<UpdateRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Update,
                .interface_ = InterfaceType::Registration,
                .response_ = false,
                .incoming_ = true,
                .notification_ = false}),
        makeTestParameter<UpdateResponse>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Update,
                .interface_ = InterfaceType::Registration,
                .response_ = true,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<DeregisterRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Deregister,
                .interface_ = InterfaceType::Registration,
                .response_ = false,
                .incoming_ = true,
                .notification_ = false}),
        makeTestParameter<DeregisterResponse>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Deregister,
                .interface_ = InterfaceType::Registration,
                .response_ = true,
                .incoming_ = false,
                .notification_ = false}),
        // Device management Messages
        makeTestParameter<CreateRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Create,
                .interface_ = InterfaceType::Device_Management,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<DeleteRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Delete,
                .interface_ = InterfaceType::Device_Management,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<DiscoverRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Discover,
                .interface_ = InterfaceType::Device_Management,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<ExecuteRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Execute,
                .interface_ = InterfaceType::Device_Management,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<ReadRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Read,
                .interface_ = InterfaceType::Device_Management,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<ReadCompositeRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Read_Composite,
                .interface_ = InterfaceType::Device_Management,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<WriteRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Write,
                .interface_ = InterfaceType::Device_Management,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<WriteAttributesRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Write_Attributes,
                .interface_ = InterfaceType::Device_Management,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<WriteCompositeRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Write_Composite,
                .interface_ = InterfaceType::Device_Management,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        // Information Reporting Messages
        makeTestParameter<ObserveRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Observe,
                .interface_ = InterfaceType::Information_Reporting,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<ObserveCompositeRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Observe_Composite,
                .interface_ = InterfaceType::Information_Reporting,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<ValueUpdated>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Notify,
                .interface_ = InterfaceType::Information_Reporting,
                .response_ = false,
                .incoming_ = true,
                .notification_ = true}),
        makeTestParameter<SendRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Send,
                .interface_ = InterfaceType::Information_Reporting,
                .response_ = false,
                .incoming_ = true,
                .notification_ = false}),
        makeTestParameter<SendResponse>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Send,
                .interface_ = InterfaceType::Information_Reporting,
                .response_ = true,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<CancelObservationRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Cancel_Observation,
                .interface_ = InterfaceType::Information_Reporting,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false}),
        makeTestParameter<CancelObserveCompositeRequest>(
            MessageTestExpectations{.endpoint_ = tested_endpoint,
                .message_type_ = MessageType::Cancel_Observation_Composite,
                .interface_ = InterfaceType::Information_Reporting,
                .response_ = false,
                .incoming_ = false,
                .notification_ = false})),
    GenerateTestName());

// NOLINTEND
