#include "Message.hpp"

#define INTERFACE_MASK 0xF0

using namespace std;

namespace LwM2M {

UnsupportedOperation::UnsupportedOperation(string const &message)
    : logic_error(message) {}

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

InterfaceType getInterfaceType(MessageType message_type) {
  switch (static_cast<int>(message_type) & INTERFACE_MASK) {
  case 0x10: {
    return InterfaceType::REGISTRATION;
  }
  case 0x20: {
    return InterfaceType::DEVICE_MANAGMENT;
  }
  case 0x30: {
    return InterfaceType::INFORMATION_REPORTING;
  }
  case 0x40: {
    return InterfaceType::BOOTSTRAP;
  }
  default: { return InterfaceType::NOT_RECOGNIZED; }
  }
}

string toString(ResponseCode type) {
  switch (type) {
  case ResponseCode::OK: {
    return "Ok";
  }
  case ResponseCode::CREATED: {
    return "Created";
  }
  case ResponseCode::DELETED: {
    return "Deleted";
  }
  case ResponseCode::CHANGED: {
    return "Changed";
  }
  case ResponseCode::CONTENT: {
    return "Content";
  }
  case ResponseCode::CONTINUE: {
    return "Continue";
  }
  case ResponseCode::BAD_REQUEST: {
    return "Bad Request";
  }
  case ResponseCode::UNAUTHORIZED: {
    return "Unauthorized";
  }
  case ResponseCode::FORBIDDEN: {
    return "Forbidden";
  }
  case ResponseCode::NOT_FOUND: {
    return "Not Found";
  }
  case ResponseCode::METHOD_NOT_ALLOWED: {
    return "Methond Not Allowed";
  }
  case ResponseCode::NOT_ACCEPTABLE: {
    return "Not Acceptable";
  }
  case ResponseCode::REQUEST_ENTITY_INCOMPLETE: {
    return "Request Entity Incomplete";
  }
  case ResponseCode::PRECOGNITION_FAILED: {
    return "Precognition Failed";
  }
  case ResponseCode::REQUEST_ENTITY_TOO_LARGE: {
    return "Request Entitity Too Large";
  }
  case ResponseCode::UNSUPPORTED_CONTENT_FORMAT: {
    return "Unsupported Content Format";
  }
  case ResponseCode::UNHANDLED:
  default: { return "Unhandled Return Code"; }
  }
}

UnsupportedResponseCode::UnsupportedResponseCode(string const &message)
    : logic_error(message) {}

UnsupportedResponseCode::UnsupportedResponseCode(
    string message_type, ResponseCode unsupported_response_code,
    unordered_set<ResponseCode> supported_response_codes)
    : logic_error(string()) {

  string error_msg = string(message_type);
  error_msg += " does not support the use of " +
               toString(unsupported_response_code) +
               " response code! It only supports: ";
  for (auto supported_response_code : supported_response_codes) {
    error_msg += toString(supported_response_code) + "\n";
  }
  *this = UnsupportedResponseCode(error_msg);
}

Message::Message(EndpointPtr endpoint, MessageType message_type,
                 InterfaceType interface, bool incomming, bool response,
                 bool notification)
    : endpoint_(endpoint), message_type_(message_type), interface_(interface),
      response_(response), incomming_(incomming), notification_(notification) {
  if (!endpoint_) {
    throw invalid_argument("Endpoint can not be null!");
  }
}

string Message::name() {
  // Provide a fallback implementation
  return "Message";
}

Response::Response(EndpointPtr endpoint, MessageType message_type,
                   InterfaceType interface, bool incomming,
                   unordered_set<ResponseCode> supported_responses,
                   ResponseCode response_code)
    : Message(endpoint, message_type, interface, incomming, true),
      supported_responses_(supported_responses), response_code_(response_code) {
  if (supported_responses_.empty()) {
    throw invalid_argument("Response must support at least 1 Response code");
  }
}

void Response::checkResponseCode(ResponseCode response_code) {
  if (supported_responses_.find(response_code) == supported_responses_.end()) {
    throw UnsupportedResponseCode(name(), response_code, supported_responses_);
  }
}

ClientRequest::ClientRequest(EndpointPtr endpoint, MessageType message_type,
                             InterfaceType interface)
    : Message(endpoint, message_type, interface, true) {}

ClientResponse::ClientResponse(EndpointPtr endpoint, MessageType message_type,
                               InterfaceType interface,
                               unordered_set<ResponseCode> supported_responses,
                               ResponseCode response_code)
    : Response(endpoint, message_type, interface, true, supported_responses,
               response_code) {}

ClientNotification::ClientNotification(EndpointPtr endpoint,
                                       MessageType message_type,
                                       InterfaceType interface)
    : Message(endpoint, message_type, interface, true, false, true) {}

ServerRequest::ServerRequest(EndpointPtr endpoint, MessageType message_type,
                             InterfaceType interface)
    : Message(endpoint, message_type, interface, false) {}

ServerResponse::ServerResponse(EndpointPtr endpoint, MessageType message_type,
                               InterfaceType interface,
                               unordered_set<ResponseCode> supported_responses,
                               ResponseCode response_code)
    : Response(endpoint, message_type, interface, false, supported_responses,
               response_code) {}

} // namespace LwM2M