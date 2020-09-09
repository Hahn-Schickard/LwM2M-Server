#ifndef __LWM2M_MESSAGE_HPP
#define __LWM2M_MESSAGE_HPP

#include "Hashers.hpp"
#include "LwM2M_DataFormat.hpp"

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace LwM2M {

struct UnsupportedOperation : public std::runtime_error {
  UnsupportedOperation(std::string const &message)
      : std::runtime_error(message) {}
};

enum class InterfaceType {
  NOT_RECOGNIZED,
  REGISTRATION = 0x10,
  DEVICE_MANAGMENT = 0x20,
  INFORMATION_REPORTING = 0x30,
  BOOTSTRAP = 0x40
};

std::string toString(InterfaceType type);

enum class MessageType {
  NOT_RECOGNIZED,
  // Registration messages
  REGISTER = 0x11,
  DEREGISTER = 0x12,
  UPDATE = 0x13,
  // Device managment messages
  READ = 0x21,
  WRITE = 0x22,
  EXECUTE = 0x23,
  CREATE = 0x24,
  DELETE = 0x25,
  WRITE_ATTRIBUTES = 0x26,
  DISCOVER = 0x27,
  READ_COMPOSITE = 0x28,
  WRITE_COMPOSITE = 0x29,
  // Information reporting messages
  OBSERVE = 0x30,
  OBSERVE_COMPOSITE = 0x31,
  CANCEL_OBSERVATION = 0x32,
  CANCEL_OBSERVATION_COMPOSITE = 0x33,
  NOTIFY = 0x34,
  SEND = 0x35
};

std::string toString(MessageType type);

struct Message {
  MessageType message_type_;
  InterfaceType interface_type_;
  bool response_;
  std::string endpoint_address_;
  unsigned int endpoint_port_;
  std::optional<uint16_t> message_id_;
  std::vector<uint8_t> token_;

  Message();
  Message(std::string endpoint_address, unsigned int endpoint_port,
          MessageType message_type);
  Message(std::string endpoint_address, unsigned int endpoint_port,
          uint16_t message_id_, std::vector<uint8_t> token,
          MessageType message_type);
};

enum class ResponseCode : int {
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
};

struct Response : Message {
  ResponseCode response_code_;
  std::shared_ptr<DataFormat> payload_;

  Response();
  Response(std::string endpoint_address, unsigned int endpoint_port,
           uint16_t message_id, std::vector<uint8_t> token,
           MessageType message_type, ResponseCode response_code);
  Response(std::string endpoint_address, unsigned int endpoint_port,
           uint16_t message_id, std::vector<uint8_t> token,
           MessageType message_type, ResponseCode response_code,
           std::shared_ptr<DataFormat> payload);

  virtual ~Response() = default;
};

} // namespace LwM2M

namespace std {
template <> struct hash<LwM2M::Message> {
  size_t operator()(const LwM2M::Message &value) const {
    return hash<int>{}(static_cast<int>(value.message_type_)) +
           hash<int>{}(static_cast<int>(value.interface_type_)) +
           hash<bool>{}(value.response_) +
           hash<string>{}(value.endpoint_address_) +
           hash<unsigned int>{}(value.endpoint_port_) +
           hash<uint16_t>{}(value.message_id_ ? value.message_id_.value() : 0) +
           hash<vector<uint8_t>>{}(value.token_);
  }
};
} // namespace std

#endif //__LWM2M_MESSAGE_HPP