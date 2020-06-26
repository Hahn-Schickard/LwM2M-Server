#ifndef __LWM2M_MESSAGE_HPP
#define __LWM2M_MESSAGE_HPP

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace LwM2M_Model {

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

struct Notify_Attripube {
  std::optional<unsigned int> minimum_period_;
  std::optional<unsigned int> maximum_period_;
  std::optional<unsigned int> greater_than_;
  std::optional<unsigned int> less_than_;
  std::optional<unsigned int> step_;
  std::optional<unsigned int> minimum_evaluation_period_;
  std::optional<unsigned int> maximum_evaluation_period_;

  Notify_Attripube(std::optional<unsigned int> minimum_period,
                   std::optional<unsigned int> maximum_period,
                   std::optional<unsigned int> greater_than,
                   std::optional<unsigned int> less_than,
                   std::optional<unsigned int> step,
                   std::optional<unsigned int> minimum_evaluation_period,
                   std::optional<unsigned int> maximum_evaluation_period);
};

struct LwM2M_Message {
  MessageType message_type_;
  InterfaceType interface_type_;
  std::string endpoint_address_;
  unsigned int endpoint_port_;
  std::vector<uint8_t> token_;

  LwM2M_Message();
  LwM2M_Message(std::string endpoint_address, unsigned int endpoint_port,
                std::vector<uint8_t> token, MessageType message_type);
};

enum LwM2M_ResponseCode {
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

struct LwM2M_Response : LwM2M_Message {
  LwM2M_ResponseCode response_code_;
  std::vector<uint8_t> payload_;

  LwM2M_Response();
  LwM2M_Response(std::string endpoint_address, unsigned int endpoint_port,
                 std::vector<uint8_t> token, MessageType message_type,
                 LwM2M_ResponseCode response_code);
  LwM2M_Response(std::string endpoint_address, unsigned int endpoint_port,
                 std::vector<uint8_t> token, MessageType message_type,
                 LwM2M_ResponseCode response_code,
                 std::vector<uint8_t> payload);
};

} // namespace LwM2M_Model

#endif //__LWM2M_MESSAGE_HPP