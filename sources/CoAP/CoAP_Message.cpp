#include "CoAP_Message.hpp"

using namespace std;
using namespace CoAP;

string CoAP::toString(MessageType type) {
  switch (type) {
  case MessageType::CONFIRMABLE: {
    return "Confirmable";
  }
  case MessageType::NON_CONFIRMABLE: {
    return "Non Confirmable";
  }
  case MessageType::ACKNOWLEDGMENT: {
    return "Acknowledgment";
  }
  case MessageType::RESET:
  default: { return "Reset"; }
  }
}

string CoAP::toString(CodeType type) {
  switch (type) {
  case CodeType::GET: {
    return "001 Request: Get";
  }
  case CodeType::POST: {
    return "002 Request: Post";
  }
  case CodeType::PUT: {
    return "003 Request: Put";
  }
  case CodeType::DELETE: {
    return "004 Request: Delete";
  }
  case CodeType::FETCH: {
    return "005 Request: Fetch";
  }
  case CodeType::iPATCH: {
    return "007 Request: iPatch";
  }
  case CodeType::CREATED: {
    return "201 Response: Created";
  }
  case CodeType::DELETED: {
    return "202 Response: Deleted";
  }
  case CodeType::CHANGED: {
    return "204 Response: Changed";
  }
  case CodeType::CONTENT: {
    return "205 Response: Content";
  }
  case CodeType::CONTINUE: {
    return "231 Response: Continue";
  }
  case CodeType::BAD_REQUEST: {
    return "400 Response: Bad Request";
  }
  case CodeType::UNAUTHORIZED: {
    return "401 Response: Unauthorized";
  }
  case CodeType::FORBIDDEN: {
    return "403 Response: Forbidden";
  }
  case CodeType::NOT_FOUND: {
    return "404 Response: Not Found";
  }
  case CodeType::METHOD_NOT_ALLOWED: {
    return "405 Response: Method Not Allowed";
  }
  case CodeType::NOT_ACCEPTABLE: {
    return "406 Response: Not Acceptable";
  }
  case CodeType::REQUEST_ENTITY_INCOMPLETE: {
    return "408 Response: Request Entity Incomplete";
  }
  case CodeType::PRECOGNITION_FAILED: {
    return "412 Response: Precognition Failed";
  }
  case CodeType::REQUEST_ENTITY_TOO_LARGE: {
    return "413 Response: Request Entity Too Large";
  }
  case CodeType::UNSUPPORTED_CONTENT_FORMAT: {
    return "415 Response: Unssuported Content";
  }
  default: { return "Unhandeled Response"; }
  }
}

CoAP_Header::CoAP_Header() : CoAP_Header(vector<char>(4, 0)) {}

CoAP_Header::CoAP_Header(vector<char> data)
    : type_(static_cast<MessageType>((0x30 & data[0]) >> 4)),
      token_length_((0x0F & data[0]) >> 0),
      code_(static_cast<CodeType>(((data[1] >> 5) & 0x07) |
                                  ((data[1] >> 0) & 0x1F))),
      message_id_((data[2] << 8) | (data[3])) {
  if (((0xC0 & data[0]) >> 6 != 1) || data.size() != 4) {
    throw Network_IO_Exception("Malformated header!");
  }
}

vector<char> CoAP_Header::toPacket() const {
  vector<char> result(4);
  result[0] = 0x40;                               // set CoAP version to 1
  result[0] = result[0] & (0xC0 | type_);         // set message type;
  result[0] = result[0] & (0xF0 | token_length_); // set token length
  result[1] = code_;
  result[2] = message_id_ >> 8;   // set id MSB
  result[3] = message_id_ & 0xFF; // set id LSB

  return result;
}

MessageType CoAP_Header::getMesageType() { return type_; }

uint8_t CoAP_Header::getTokenLenght() { return token_length_; }

CodeType CoAP_Header::getCodeType() { return code_; }

uint16_t CoAP_Header::getMessageID() { return message_id_; }

CoAP_Message::CoAP_Message()
    : CoAP_Message(string(), CoAP_Header(), vector<char>()) {}

CoAP_Message::CoAP_Message(string receiver_ip, CoAP_Header header_data,
                           vector<char> body_data)
    : receiver_(receiver_ip), header_(move(header_data)),
      body_(move(body_data)) {}

vector<char> CoAP_Message::toPacket() {
  auto result = header_.toPacket();
  result.insert(result.end(), body_.begin(), body_.end());
  return result;
}

string CoAP_Message::getReceiver() { return receiver_; }

CoAP_Header CoAP_Message::getHeader() { return header_; }

vector<char> CoAP_Message::getBody() { return body_; }