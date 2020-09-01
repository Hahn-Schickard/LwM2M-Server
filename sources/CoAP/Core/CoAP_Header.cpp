#include "CoAP_Header.hpp"

#include <random>

#define MAX_MESSAGE_ID 65535

using namespace std;
namespace CoAP {

string toString(MessageType type) {
  string result;
  switch (type) {
  case MessageType::CONFIRMABLE: {
    result = "Confirmable";
    break;
  }
  case MessageType::NON_CONFIRMABLE: {
    result = "Non Confirmable";
    break;
  }
  case MessageType::ACKNOWLEDGMENT: {
    result = "Acknowledgment";
    break;
  }
  case MessageType::RESET: {
    result = "Reset";
    break;
  }
  default: {
    result = "Malformated";
    break;
  }
  }
  return result;
}

string toString(CodeType type) {
  string result;
  switch (type) {
  case CodeType::GET: {
    result = "001 Request: Get";
    break;
  }
  case CodeType::POST: {
    result = "002 Request: Post";
    break;
  }
  case CodeType::PUT: {
    result = "003 Request: Put";
    break;
  }
  case CodeType::DELETE: {
    result = "004 Request: Delete";
    break;
  }
  case CodeType::FETCH: {
    result = "005 Request: Fetch";
    break;
  }
  case CodeType::iPATCH: {
    result = "007 Request: iPatch";
    break;
  }
  case CodeType::OK: {
    result = "200 Response: OK";
    break;
  }
  case CodeType::CREATED: {
    result = "201 Response: Created";
    break;
  }
  case CodeType::DELETED: {
    result = "202 Response: Deleted";
    break;
  }
  case CodeType::CHANGED: {
    result = "204 Response: Changed";
    break;
  }
  case CodeType::CONTENT: {
    result = "205 Response: Content";
    break;
  }
  case CodeType::CONTINUE: {
    result = "231 Response: Continue";
    break;
  }
  case CodeType::BAD_REQUEST: {
    result = "400 Response: Bad Request";
    break;
  }
  case CodeType::UNAUTHORIZED: {
    result = "401 Response: Unauthorized";
    break;
  }
  case CodeType::FORBIDDEN: {
    result = "403 Response: Forbidden";
    break;
  }
  case CodeType::NOT_FOUND: {
    result = "404 Response: Not Found";
    break;
  }
  case CodeType::METHOD_NOT_ALLOWED: {
    result = "405 Response: Method Not Allowed";
    break;
  }
  case CodeType::NOT_ACCEPTABLE: {
    result = "406 Response: Not Acceptable";
    break;
  }
  case CodeType::REQUEST_ENTITY_INCOMPLETE: {
    result = "408 Response: Request Entity Incomplete";
    break;
  }
  case CodeType::PRECOGNITION_FAILED: {
    result = "412 Response: Precognition Failed";
    break;
  }
  case CodeType::REQUEST_ENTITY_TOO_LARGE: {
    result = "413 Response: Request Entity Too Large";
    break;
  }
  case CodeType::UNSUPPORTED_CONTENT_FORMAT: {
    result = "415 Response: Unssuported Content";
    break;
  }
  case CodeType::CSM: {
    result = "701 Signaling Code: CSM";
    break;
  }
  case CodeType::PING: {
    result = "702 Signaling Code: Ping";
    break;
  }
  case CodeType::PONG: {
    result = "703 Signaling Code: Pong";
    break;
  }
  case CodeType::RELEASE: {
    result = "704 Signaling Code: Reakeas";
    break;
  }
  case CodeType::ABORT: {
    result = "705 Signaling Code: Abort";
    break;
  }
  default: {
    result = "Unhandeled Response";
    break;
  }
  }
  return result;
}

uint16_t generateMessageID() {
  random_device randomization_device;
  mt19937 generator(randomization_device());

  uniform_int_distribution<> random_number_pool_(0, MAX_MESSAGE_ID);
  return random_number_pool_(generator);
}

Header::Header() {}

Header::Header(vector<uint8_t> data) {
  if (!data.empty()) {
    int coap_ver = (0xC0 & data[0]) >> 6;
    if ((coap_ver != 1)) {
      string error_msg = "Malformated header: CoAP version " +
                         to_string(coap_ver) + " is not supported.";
      throw Network_IO_Exception(error_msg);
    } else if (data.size() != 4) {
      string error_msg =
          "Malformated header: Expected a 4 byte header, received: " +
          to_string(data.size());
      throw Network_IO_Exception(error_msg);
    }

    type_ = static_cast<MessageType>((0x30 & data[0]) >> 4);
    token_length_ = 0x0F & data[0];
    code_ = static_cast<CodeType>(data[1]);
    message_id_ = (data[2] << 8) | (data[3]);
  }
}

Header::Header(MessageType type, uint8_t message_length, CodeType code_type)
    : Header(type, message_length, code_type, generateMessageID()) {}

Header::Header(MessageType type, uint8_t message_length, CodeType code_type,
               uint16_t message_id)
    : type_(type), token_length_(message_length), code_(code_type),
      message_id_(message_id) {}

bool operator==(const Header &lhs, const Header &rhs) {
  return lhs.toPacket() == rhs.toPacket();
}

vector<uint8_t> Header::toPacket() const {
  vector<uint8_t> result(4);
  result[0] = 0x40; // set CoAP version to 1
  result[0] = result[0] | (static_cast<int>(type_) << 4); // set message type;
  result[0] = result[0] | token_length_;                  // set token length
  result[1] = static_cast<int>(code_);
  result[2] = message_id_ >> 8;   // set id MSB
  result[3] = message_id_ & 0xFF; // set id LSB

  return result;
}

MessageType Header::getMesageType() const { return type_; }

uint8_t Header::getTokenLenght() const { return token_length_; }

CodeType Header::getCodeType() const { return code_; }

uint16_t Header::getMessageID() const { return message_id_; }

} // namespace CoAP