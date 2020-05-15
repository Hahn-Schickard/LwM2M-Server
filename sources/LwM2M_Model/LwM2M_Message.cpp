#include "LwM2M_Message.hpp"

#include <stdexcept>

using namespace std;

#define INTERFACE_MASK 0xF0

namespace LwM2M_Model {

string toString(InterfaceType type) {
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

string toString(MessageType type) {
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

Notify_Attripube::Notify_Attripube(
    optional<unsigned int> minimum_period,
    optional<unsigned int> maximum_period, optional<unsigned int> greater_than,
    optional<unsigned int> less_than, optional<unsigned int> step,
    optional<unsigned int> minimum_evaluation_period,
    optional<unsigned int> maximum_evaluation_period)
    : minimum_period_(move(minimum_period)),
      maximum_period_(move(maximum_period)), greater_than_(move(greater_than)),
      less_than_(move(less_than)), step_(move(step)),
      minimum_evaluation_period_(move(minimum_evaluation_period)),
      maximum_evaluation_period_(move(maximum_evaluation_period)) {}

LwM2M_Message::LwM2M_Message(InterfaceType interface_type,
                             MessageType message_type)
    : interface_type_(interface_type), message_type_(message_type) {
  if (interface_type_ != (message_type_ & INTERFACE_MASK)) {
    string error_msg = toString(interface_type_) + "does not supprot " +
                       toString(message_type_) + " message type";
    throw logic_error(error_msg);
  }
}

InterfaceType LwM2M_Message::getInterfaceType() { return interface_type_; }

MessageType LwM2M_Message::getMessageType() { return message_type_; }

} // namespace LwM2M_Model
