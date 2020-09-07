#include "CoAP_Message.hpp"
#include "Threadsafe_Unique_Queue.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <ostream>
#include <string>

#define BYTE_MSB_MASK 0xF0
#define BYTE_LSB_MASK 0xF

using namespace std;
using namespace CoAP;

using MessagePtr = std::unique_ptr<Message>;

Message
makeMessage(string ip, unsigned int port, vector<uint8_t> token,
            uint16_t msg_id, MessageType msg_type, CodeType code_type,
            vector<shared_ptr<CoAP::Option>> options =
                vector<shared_ptr<CoAP::Option>>(),
            shared_ptr<PayloadFormat> payload = shared_ptr<PayloadFormat>()) {
  auto header = Header(msg_type, token.size(), code_type, msg_id);
  return Message(ip, port, header, token, options, payload);
}

string toString(Header header) {
  return string("Header:  \n") + string(" Message type: ") +
         toString(header.getMesageType()) + string("\n") +
         string(" Token lenght: ") + to_string(header.getTokenLenght()) +
         string("\n") + string(" Code type: ") +
         toString(header.getCodeType()) + string("\n") +
         string(" Message id: ") + to_string(header.getMessageID());
}

string toString(vector<uint8_t> bytes) {
  char const hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'F'};
  string hex;
  for (size_t i = 0; i < bytes.size(); i++) {
    hex += hex_chars[(bytes[i] & BYTE_MSB_MASK) >> 4];
    hex += hex_chars[(bytes[i] & BYTE_LSB_MASK)];
  }
  return hex;
}

string toString(shared_ptr<Option> option) {
  return toString(option->getOptionNumber()) + toString(option->getValue());
}

string toString(vector<shared_ptr<Option>> options) {
  string result;
  for (auto option : options) {
    result += toString(option) + string("\n");
  }
  return result;
}

ostream &operator<<(ostream &os, const Message &msg) {
  return os << "Address: " << msg.getReceiverIP() << ":"
            << msg.getReceiverPort() << endl
            << toString(msg.getHeader()) << endl
            << " Token: " << toString(msg.getToken()) << endl
            << toString(msg.getOptions())
            << " with payload: " << toString(msg.getBody()->getBytes()) << endl;
}

class CoAPMessageQueueTests : public ::testing::Test {
protected:
  void SetUp() override {
    queue = make_unique<ThreadsafeUniqueQueue<Message>>();
  }
  unique_ptr<ThreadsafeUniqueQueue<Message>> queue;
};

TEST_F(CoAPMessageQueueTests, returnsEmptyPtrForEmptyQueue) {
  EXPECT_FALSE(queue->try_pop());
  EXPECT_EQ(queue->try_pop().get(), nullptr);
}

TEST_F(CoAPMessageQueueTests, canPushNewValue) {
  Message expected =
      makeMessage("1.1.1.1", 8080, vector<uint8_t>{1, 2, 3, 4, 5, 6, 7, 8}, 12,
                  MessageType::ACKNOWLEDGMENT, CodeType::POST);
  EXPECT_NO_THROW(queue->push(make_unique<Message>(expected)));
  Message tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested, expected);
}

TEST_F(CoAPMessageQueueTests, canNotPushDuplicateValue) {
  Message expected =
      makeMessage("1.1.1.1", 8080, vector<uint8_t>{1, 2, 3, 4, 5, 6, 7, 8}, 12,
                  MessageType::ACKNOWLEDGMENT, CodeType::POST);
  EXPECT_NO_THROW(queue->push(expected));
  EXPECT_NO_THROW(queue->push(expected));
  EXPECT_NO_THROW(queue->push(makeMessage(
      "1.1.1.1", 8080, vector<uint8_t>{9, 10, 11, 12, 13, 14, 15, 16}, 40,
      MessageType::ACKNOWLEDGMENT, CodeType::GET)));
  Message tested1;
  EXPECT_TRUE(queue->try_pop(tested1));
  EXPECT_EQ(tested1, expected);
  Message tested2;
  EXPECT_TRUE(queue->try_pop(tested2));
  EXPECT_NE(tested2, expected);
  EXPECT_FALSE(queue->try_pop(tested1));
}

TEST_F(CoAPMessageQueueTests, canNotPushDuplicateUniqueValue) {
  Message expected =
      makeMessage("1.1.1.1", 8080, vector<uint8_t>{1, 2, 3, 4, 5, 6, 7, 8}, 12,
                  MessageType::ACKNOWLEDGMENT, CodeType::POST);
  EXPECT_NO_THROW(queue->push(make_unique<Message>(expected)));
  EXPECT_NO_THROW(queue->push(make_unique<Message>(expected)));
  EXPECT_NO_THROW(queue->push(make_unique<Message>(makeMessage(
      "1.1.1.1", 8080, vector<uint8_t>{9, 10, 11, 12, 13, 14, 15, 16}, 40,
      MessageType::ACKNOWLEDGMENT, CodeType::GET))));
  Message tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested, expected);
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_NE(tested, expected);
  EXPECT_FALSE(queue->try_pop(tested));
}