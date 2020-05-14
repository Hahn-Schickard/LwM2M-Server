#include "LwM2M_Message.hpp"

#include <stdexcept>

using namespace std;

#define INTERFACE_MASK 0xF0

namespace LwM2M_Model {

std::string toString(InterfaceType type) {
  switch (type) {
  case InterfaceType::REGISTRATION: {
    return "Registration Interface";
  }
  case InterfaceType::DEVICE_MANAGMENT: {
    return "Device Managment Interface";
  }
  case InterfaceType::INFORMATION_REPORTING: {
    return "Information Reporting Interface";
  }
  default: { return "Unrecognized Interface"; }
  }
}

std::string toString(MessageType type) {
  switch (type) {
  case MessageType::REGISTER: {
    return "Registration";
  }
  case MessageType::DEREGISTER: {
    return "De-Registration";
  }
  case MessageType::UPDATE: {
    return "Update";
  }
  case MessageType::READ: {
    return "Read";
  }
  case MessageType::WRITE: {
    return "Write";
  }
  case MessageType::EXECUTE: {
    return "Execute";
  }
  case MessageType::CREATE: {
    return "Create";
  }
  case MessageType::DELETE: {
    return "Delete";
  }
  case MessageType::WRITE_ATTRIBUTES: {
    return "Write Attributes";
  }
  case MessageType::WRITE_COMPOSITE: {
    return "Write Composite";
  }
  case MessageType::OBSERVE: {
    return "Observe";
  }
  case MessageType::OBSERVE_COMPOSITE: {
    return "Observe Composite";
  }
  case MessageType::CANCEL_OBSERVATION: {
    return "Cancel Observation";
  }
  case MessageType::CANCEL_OBSERVATION_COMPOSITE: {
    return "Cancel Observation Composite";
  }
  case MessageType::NOTIFY: {
    return "Notify";
  }
  case MessageType::SEND: {
    return "Send";
  }
  default: { return "Unhandled"; };
  }
}

LwM2M_Message::LwM2M_Message(InterfaceType interface_type,
                             MessageType message_type, std::string message)
    : interface_type_(interface_type), message_type_(message_type),
      message_(message) {
  if (interface_type_ != (message_type_ & INTERFACE_MASK)) {
    std::string error_msg = toString(interface_type_) + "does not supprot " +
                            toString(message_type_) + " message type";
    throw std::logic_error(error_msg);
  }
}

InterfaceType LwM2M_Message::getInterfaceType() { return interface_type_; }

MessageType LwM2M_Message::getMessageType() { return message_type_; }

std::string LwM2M_Message::getMessage() { return message_; }

} // namespace LwM2M_Model
