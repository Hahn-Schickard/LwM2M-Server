#ifndef __COAP_MESSAGE_DEFINITION_HPP
#define __COAP_MESSAGE_DEFINITION_HPP

#include <cstdint>
#include <deque>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace CoAP {
struct Network_IO_Exception : public std::runtime_error {
  Network_IO_Exception(std::string const &message)
      : std::runtime_error(message) {}
};

typedef enum MessageTypeEnum {
  CONFIRMABLE = 0,
  NON_CONFIRMABLE = 1,
  ACKNOWLEDGMENT = 2,
  RESET = 3
} MessageType;

std::string toString(MessageType type);

typedef enum CodeTypeEnum {
  GET = 0x01,
  POST = 0x02,
  PUT = 0x03,
  DELETE = 0x04,
  FETCH = 0x05,
  iPATCH = 0x07,
  OK = 0x40,
  CREATED = 0x41,
  DELETED = 0x42,
  CHANGED = 0x44,
  CONTENT = 0x45,
  CONTINUE = 0x5F,
  BAD_REQUEST = 0x80,
  UNAUTHORIZED = 0x81,
  FORBIDDEN = 0x83,
  NOT_FOUND = 0x84,
  METHOD_NOT_ALLOWED = 0x85,
  NOT_ACCEPTABLE = 0x86,
  REQUEST_ENTITY_INCOMPLETE = 0x88,
  PRECOGNITION_FAILED = 0x8C,
  REQUEST_ENTITY_TOO_LARGE = 0x8D,
  UNSUPPORTED_CONTENT_FORMAT = 0x8F,
  UNHANDLED
} CodeType;

std::string toString(CodeType type);

class CoAP_Header {
  MessageType type_;
  uint8_t token_length_;
  CodeType code_;
  uint16_t message_id_;

public:
  CoAP_Header();
  CoAP_Header(std::vector<uint8_t> data);
  CoAP_Header(MessageType type, uint8_t message_length, CodeType code_type,
              uint16_t message_id);

  std::vector<uint8_t> toPacket();
  MessageType getMesageType();
  uint8_t getTokenLenght();
  CodeType getCodeType();
  uint16_t getMessageID();
};

typedef enum OptionNumberEnum {
  RESERVED = 0,
  IF_MATCH = 1,
  URI_HOST = 3,
  ETAG = 4,
  IF_NONE_MATCH = 5,
  URI_PORT = 7,
  LOCATION_PATH = 8,
  URI_PATH = 11,
  CONTENT_FORMAT = 12,
  MAX_AGE = 14,
  URI_QUERY = 15,
  ACCEPT = 17,
  LOCATION_QUERY = 20,
  PROXY_URI = 35,
  PROXY_SCHEME = 39,
  SIZE_1 = 60
} OptionNumber;

class CoAP_Option {
  OptionNumber option_number_;
  std::string value_;
  size_t option_size_;

public:
  CoAP_Option();
  CoAP_Option(std::optional<CoAP_Option> previous, std::deque<uint8_t> options);

  size_t size();

  OptionNumber getOptionNumber();
  std::string getValue();
};

class CoAP_Message {
  std::string receiver_ip_;
  unsigned int receiver_port_;
  CoAP_Header header_;
  std::optional<std::vector<uint8_t>> token_;
  std::optional<std::vector<CoAP_Option>> options_;
  std::vector<uint8_t> body_;

public:
  CoAP_Message();
  CoAP_Message(std::string receiver_ip, unsigned int receiver_port,
               CoAP_Header header_data, std::vector<uint8_t> body_data);

  std::vector<uint8_t> toPacket();
  std::string getReceiverIP();
  unsigned int getReceiverPort();
  CoAP_Header &getHeader();
  std::optional<std::vector<uint8_t>> getToken();
  std::optional<std::vector<CoAP_Option>> getOptions();
  std::vector<uint8_t> getBody();
};
} // namespace CoAP
#endif //__COAP_MESSAGE_DEFINITION_HPP