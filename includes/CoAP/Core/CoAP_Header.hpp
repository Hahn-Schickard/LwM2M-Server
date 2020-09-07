#ifndef __COAP_HEADER_DEFINITION_HPP
#define __COAP_HEADER_DEFINITION_HPP

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace CoAP {

struct Network_IO_Exception : public std::runtime_error {
  Network_IO_Exception(std::string const &message)
      : std::runtime_error(message) {}
};

enum class MessageType : uint8_t {
  CONFIRMABLE = 0,
  NON_CONFIRMABLE = 1,
  ACKNOWLEDGMENT = 2,
  RESET = 3
};

std::string toString(MessageType type);

enum class CodeType : uint8_t {
  // Requests
  GET = 0x01,
  POST = 0x02,
  PUT = 0x03,
  DELETE = 0x04,
  FETCH = 0x05,
  iPATCH = 0x07,
  // Responses
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
  CSM = 0xE1,
  PING = 0xE2,
  PONG = 0xE3,
  RELEASE = 0xE4,
  ABORT = 0xE5,
  UNHANDLED
};

std::string toString(CodeType type);

class Header {
  MessageType type_;
  uint8_t token_length_;
  CodeType code_;
  uint16_t message_id_;

public:
  Header();
  Header(std::vector<uint8_t> data);
  Header(MessageType type, uint8_t message_length, CodeType code_type);
  Header(MessageType type, uint8_t message_length, CodeType code_type,
         uint16_t message_id);

  friend bool operator==(const Header &lhs, const Header &rhs);

  std::vector<uint8_t> toPacket() const;
  MessageType getMesageType() const;
  uint8_t getTokenLenght() const;
  CodeType getCodeType() const;
  uint16_t getMessageID() const;
};
} // namespace CoAP

namespace std {
template <> struct hash<CoAP::Header> {
  size_t operator()(const CoAP::Header &value) const {
    auto bytes = value.toPacket();
    size_t result = 0;
    for (size_t i = 0; i < bytes.size(); i++) {
      auto hashed_byte = hash<uint8_t>{}(bytes[i]);
      result |= hashed_byte << (i * 8);
    }
    return result;
  }
};
} // namespace std

#endif //__COAP_HEADER_DEFINITION_HPP