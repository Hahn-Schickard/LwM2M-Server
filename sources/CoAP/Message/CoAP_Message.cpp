#include "CoAP_Message.hpp"
#include "Option_Builder.hpp"

#include <exception>

#define PAYLOAD_MARKER_SIZE 1

using namespace std;
namespace CoAP {

string toString(MessageType type) {
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

string toString(CodeType type) {
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
  case CodeType::OK: {
    return "200 Response: OK";
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

CoAP_Header::CoAP_Header() : CoAP_Header(vector<uint8_t>(4, 0)) {}

CoAP_Header::CoAP_Header(vector<uint8_t> data) {
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
  token_length_ = (0x0F & data[0]) >> 0;
  code_ = static_cast<CodeType>(data[1]);
  message_id_ = (data[2] << 8) | (data[3]);
}

CoAP_Header::CoAP_Header(MessageType type, uint8_t message_length,
                         CodeType code_type, uint16_t message_id)
    : type_(type), token_length_(message_length), code_(code_type),
      message_id_(message_id) {}

vector<uint8_t> CoAP_Header::toPacket() {
  vector<uint8_t> result(4);
  result[0] = 0x40;                      // set CoAP version to 1
  result[0] = result[0] | (type_ << 4);  // set message type;
  result[0] = result[0] | token_length_; // set token length
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
    : CoAP_Message(string(), 0, CoAP_Header(), vector<uint8_t>()) {}

CoAP_Message::CoAP_Message(string receiver_ip, unsigned int receiver_port,
                           CoAP_Header header_data, vector<uint8_t> body_data)
    : receiver_ip_(receiver_ip), receiver_port_(receiver_port),
      header_(move(header_data)) {
  deque<uint8_t> payload(body_data.begin(), body_data.end());

  if (header_.getTokenLenght() != 0) {
    uint8_t index = header_.getTokenLenght();
    while (index != 0) {
      token_->push_back(payload.front());
      payload.pop_front();
      index--;
    }
  } else {
    token_ = nullopt;
  }

  auto previous = shared_ptr<CoAP_Option>();
  shared_ptr<CoAP_Option> current;
  do {
    try {
      if (!payload.empty()) {
        current = build(previous, payload);
        options_.push_back(current);
        previous = current;
        payload.erase(payload.begin(), payload.begin() + current->size());
      } else {
        break; // received a message without a payload
      }
    } catch (PayloadMarkerDetected &exp) {
      payload.erase(payload.begin(), payload.begin() + PAYLOAD_MARKER_SIZE);
      break;
    }
  } while (true);

  if (!payload.empty()) {
    body_ = vector<uint8_t>(payload.begin(), payload.end());
  } else {
    body_ = vector<uint8_t>(0);
  }
}

vector<uint8_t> CoAP_Message::toPacket() {
  auto result = header_.toPacket();
  result.insert(result.end(), body_.begin(), body_.end());
  return result;
}

string CoAP_Message::getReceiverIP() { return receiver_ip_; }

unsigned int CoAP_Message::getReceiverPort() { return receiver_port_; }

CoAP_Header &CoAP_Message::getHeader() { return header_; }

optional<vector<uint8_t>> CoAP_Message::getToken() { return token_; }

vector<shared_ptr<CoAP_Option>> CoAP_Message::getOptions() { return options_; }

vector<uint8_t> CoAP_Message::getBody() { return body_; }
} // namespace CoAP