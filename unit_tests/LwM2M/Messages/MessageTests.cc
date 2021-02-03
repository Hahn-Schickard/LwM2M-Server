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

template <typename T>
TestParameter makePassingTest(const TestExpectations &valid_expectations) {
  assert((is_base_of<Message, T>::value));

  shared_ptr<T> message;
  try {
    message = make_shared<T>(make_shared<Endpoint>());
  } catch (exception &ex) {
    cerr << "Caught an unhandeled exception while building "
         << toString(valid_expectations.message_type_)
         << " message test. Exception was: " << ex.what() << endl;
    throw ex;
  }
  auto expectations = make_shared<TestExpectations>(valid_expectations);

  return TestParameter(move(message), move(expectations));
}

vector<TestParameter> parameters{
    // Registration Messages
    TestParameter(makePassingTest<RegisterRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::REGISTER,
                         .interface_ = InterfaceType::REGISTRATION,
                         .response_ = false,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<RegisterResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::REGISTER,
                         .interface_ = InterfaceType::REGISTRATION,
                         .response_ = true,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<UpdateRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::UPDATE,
                         .interface_ = InterfaceType::REGISTRATION,
                         .response_ = false,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<UpdateResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::UPDATE,
                         .interface_ = InterfaceType::REGISTRATION,
                         .response_ = true,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<DeregisterRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::DEREGISTER,
                         .interface_ = InterfaceType::REGISTRATION,
                         .response_ = false,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<DeregisterResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::DEREGISTER,
                         .interface_ = InterfaceType::REGISTRATION,
                         .response_ = true,
                         .incomming_ = false,
                         .notification_ = false})),
    // Device managment Messages
    TestParameter(makePassingTest<CreateRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::CREATE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = false,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<CreateResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::CREATE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = true,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<DeleteRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::DELETE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = false,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<DeleteResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::DELETE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = true,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<DiscoverRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::DISCOVER,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = false,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<DiscoverResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::DISCOVER,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = true,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<ExecuteRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::EXECUTE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = false,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<ExecuteResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::EXECUTE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = true,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<ReadRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::READ,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = false,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<ReadResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::READ,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = true,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<ReadComopositeRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::READ_COMPOSITE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = false,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<ReadComopositeResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::READ_COMPOSITE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = true,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<WriteRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::WRITE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = false,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<WriteResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::WRITE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = true,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<WriteAttributesRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::WRITE_ATTRIBUTES,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = false,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<WriteAttributesResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::WRITE_ATTRIBUTES,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = true,
                         .incomming_ = true,
                         .notification_ = false})),
    TestParameter(makePassingTest<WriteComopositeRequest>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::WRITE_COMPOSITE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = false,
                         .incomming_ = false,
                         .notification_ = false})),
    TestParameter(makePassingTest<WriteComopositeResponse>(
        TestExpectations{.endpoint_ = make_shared<Endpoint>(),
                         .message_type_ = MessageType::WRITE_COMPOSITE,
                         .interface_ = InterfaceType::DEVICE_MANAGMENT,
                         .response_ = true,
                         .incomming_ = true,
                         .notification_ = false}))};

INSTANTIATE_TEST_SUITE_P(BaseFieldTests, MessageTest,
                         testing::ValuesIn(parameters), GenerateTestName());