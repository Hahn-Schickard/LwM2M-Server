#include "CoAP_Message.hpp"

#include <exception>

#define BYTE_MSB_MASK 0xF0
#define BYTE_LSB_MASK 0xF
#define BYTE_LONG 0xD
#define BYTE_LONG_OFFSET 0xD
#define SHORT_LONG 0xE
#define SHORT_LONG_OFFSET 0x10D
#define PAYLOAD_DELTA 0xF
#define PAYLOAD_MARKER 0xFF
#define PAYLOAD_MARKER_SIZE 1

using namespace std;
namespace CoAP {

struct PayloadMarkerDetected : public exception {
  const char *what() const throw() { return "Payload Marker Detected"; }
};

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

string toHexString(deque<uint8_t> bytes, size_t ammount) {
  char const hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'F'};
  string hex;
  for (size_t i = 0; i < ammount; i++) {
    hex += hex_chars[(bytes[i] & BYTE_MSB_MASK) >> 4];
    hex += hex_chars[(bytes[i] & BYTE_LSB_MASK)];
  }
  return hex;
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

OptionNumber makeOptionNumber(unsigned int number) {
  switch (number) {
  case 1:
    return OptionNumber::IF_MATCH;
  case 3:
    return OptionNumber::URI_HOST;
  case 4:
    return OptionNumber::ETAG;
  case 5:
    return OptionNumber::IF_NONE_MATCH;
  case 7:
    return OptionNumber::URI_PORT;
  case 8:
    return OptionNumber::LOCATION_PATH;
  case 11:
    return OptionNumber::URI_PATH;
  case 12:
    return OptionNumber::CONTENT_FORMAT;
  case 15:
    return OptionNumber::MAX_AGE;
  case 17:
    return OptionNumber::ACCEPT;
  case 20:
    return OptionNumber::LOCATION_QUERY;
  case 35:
    return OptionNumber::PROXY_URI;
  case 39:
    return OptionNumber::PROXY_SCHEME;
  case 60:
    return OptionNumber::SIZE_1;
  default: {
    string error_msg = "Received an unhandled CoAP option: " + number;
    throw Network_IO_Exception(error_msg);
  }
  }
}

CoAP_Option::CoAP_Option() : CoAP_Option(nullopt, deque<uint8_t>()) {}

CoAP_Option::CoAP_Option(std::optional<CoAP_Option> previous,
                         deque<uint8_t> option)
    : option_number_(OptionNumber::RESERVED), value_(string()),
      option_size_(0) {
  unsigned short delta;
  unsigned short lentgth;

  if (!option.empty()) {
    if (option[0] != PAYLOAD_MARKER) {
      option_size_ = 1;

      uint8_t msb = option[0] & BYTE_MSB_MASK;
      switch (msb) {
      case BYTE_LONG: {
        delta = option[1] + BYTE_LONG_OFFSET;
        option_size_++;
        break;
      }
      case SHORT_LONG: {
        delta = option[1] + option[2] + SHORT_LONG_OFFSET;
        option_size_ += 2;
        break;
      }
      case PAYLOAD_DELTA: {
        string error_msg =
            "Malformated CoAP option: Delta set to 0xF, but the byte " +
            toHexString(option, 2) + " is not equal to 0xFF";
        throw Network_IO_Exception(error_msg);
      }
      default: {
        delta = msb;
        break;
      }
      }

      uint8_t lsb = option[0] & BYTE_LSB_MASK;
      switch (lsb) {
      case BYTE_LONG: {
        lentgth = option[1] + BYTE_LONG_OFFSET;
        option_size_++;
        break;
      }
      case SHORT_LONG: {
        lentgth = option[1] + option[2] + SHORT_LONG_OFFSET;
        option_size_ += 2;
        break;
      }
      case PAYLOAD_DELTA: {
        string error_msg =
            "Malformated CoAP option: Lenght is set to 0xF, but the byte " +
            toHexString(option, 2) + " is not equal to 0xFF";
        throw Network_IO_Exception(error_msg);
      }
      default: {
        lentgth = lsb;
        break;
      }
      }

      if (previous) {
        option_number_ = makeOptionNumber(delta + previous->getOptionNumber());
      } else {
        option_number_ = makeOptionNumber(delta);
      }

      if (lentgth != 0) {
        value_ = string(option.begin(), option.begin() + lentgth);
        option_size_ += lentgth;
      }
    } else {
      throw PayloadMarkerDetected();
    }
  }
}

size_t CoAP_Option::size() { return option_size_; }

OptionNumber CoAP_Option::getOptionNumber() { return option_number_; }

std::string CoAP_Option::getValue() { return value_; }

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

  optional<CoAP_Option> previous = nullopt;
  optional<CoAP_Option> current;
  do {
    try {
      if (!payload.empty()) {
        current = CoAP_Option(previous, payload);
        options_->push_back(current.value());
        previous = current;
        payload.erase(payload.begin(), payload.begin() + current->size());
      } else {
        throw Network_IO_Exception(
            "Received a CoAP message without payload marker.");
      }
    } catch (PayloadMarkerDetected &exp) {
      payload.erase(payload.begin(), payload.begin() + PAYLOAD_MARKER_SIZE);
      break;
    }
  } while (true);

  body_ = vector<uint8_t>(payload.begin(), payload.end());
}

vector<uint8_t> CoAP_Message::toPacket() {
  auto result = header_.toPacket();
  result.insert(result.end(), body_.begin(), body_.end());
  return result;
}

string CoAP_Message::getReceiverIP() { return receiver_ip_; }

unsigned int CoAP_Message::getReceiverPort() { return receiver_port_; }

CoAP_Header &CoAP_Message::getHeader() { return header_; }

std::optional<std::vector<uint8_t>> CoAP_Message::getToken() { return token_; }

std::optional<std::vector<CoAP_Option>> CoAP_Message::getOptions() {
  return options_;
}

vector<uint8_t> CoAP_Message::getBody() { return body_; }
} // namespace CoAP