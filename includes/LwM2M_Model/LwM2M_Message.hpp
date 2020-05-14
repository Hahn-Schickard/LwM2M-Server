#ifndef __LWM2M_MESSAGE_HPP
#define __LWM2M_MESSAGE_HPP

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace LwM2M_Model {

struct UnsupportedOperation : public std::runtime_error {
  UnsupportedOperation(std::string const &message)
      : std::runtime_error(message) {}
};

typedef enum InterfaceTypeEnum {
  REGISTRATION = 0x10,
  DEVICE_MANAGMENT = 0x20,
  INFORMATION_REPORTING = 0x30
} InterfaceType;

std::string toString(InterfaceType type);

typedef enum MessageTypeEnum {
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
} MessageType;

std::string toString(MessageType type);

class LwM2M_Message {
  InterfaceType interface_type_;
  MessageType message_type_;
  std::string message_;

public:
  LwM2M_Message(InterfaceType interface_type, MessageType message_type,
                std::string message);

  InterfaceType getInterfaceType();
  MessageType getMessageType();
  std::string getMessage();
};

} // namespace LwM2M_Model

#endif //__LWM2M_MESSAGE_HPP