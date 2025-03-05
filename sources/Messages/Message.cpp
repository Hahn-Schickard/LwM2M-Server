#include "Message.hpp"
using namespace std;

namespace LwM2M {

static constexpr uint8_t INTERFACE_MASK = 0xF0;

UnsupportedOperation::UnsupportedOperation(const string& message)
    : logic_error(message) {}

string toString(InterfaceType type) {
  switch (type) {
  case InterfaceType::Registration: {
    return "Registration Interface";
  }
  case InterfaceType::Device_Management: {
    return "Device Management Interface";
  }
  case InterfaceType::Information_Reporting: {
    return "Information Reporting Interface";
  }
  default: {
    return "Unrecognized Interface";
  }
  }
}

string toString(MessageType type) {
  switch (type) {
  case MessageType::Register: {
    return "Registration";
  }
  case MessageType::Deregister: {
    return "De-Registration";
  }
  case MessageType::Update: {
    return "Update";
  }
  case MessageType::Read: {
    return "Read";
  }
  case MessageType::Write: {
    return "Write";
  }
  case MessageType::Execute: {
    return "Execute";
  }
  case MessageType::Create: {
    return "Create";
  }
  case MessageType::Delete: {
    return "Delete";
  }
  case MessageType::Write_Attributes: {
    return "Write Attributes";
  }
  case MessageType::Discover: {
    return "Discover";
  }
  case MessageType::Read_Composite: {
    return "Read Composite";
  }
  case MessageType::Write_Composite: {
    return "Write Composite";
  }
  case MessageType::Observe: {
    return "Observe";
  }
  case MessageType::Observe_Composite: {
    return "Observe Composite";
  }
  case MessageType::Cancel_Observation: {
    return "Cancel Observation";
  }
  case MessageType::Cancel_Observation_Composite: {
    return "Cancel Observation Composite";
  }
  case MessageType::Notify: {
    return "Notify";
  }
  case MessageType::Send: {
    return "Send";
  }
  default: {
    return "Unhandled";
  };
  }
}

static constexpr uint8_t REGISTRATION_INTERFACE = 0x10;
static constexpr uint8_t MANAGEMENT_INTERFACE = 0x20;
static constexpr uint8_t REPORTING_INTERFACE = 0x30;
static constexpr uint8_t BOOTSTRAP_INTERFACE = 0x40;

InterfaceType getInterfaceType(MessageType message_type) {
  switch (static_cast<int>(message_type) & INTERFACE_MASK) {
  case REGISTRATION_INTERFACE: {
    return InterfaceType::Registration;
  }
  case MANAGEMENT_INTERFACE: {
    return InterfaceType::Device_Management;
  }
  case REPORTING_INTERFACE: {
    return InterfaceType::Information_Reporting;
  }
  case BOOTSTRAP_INTERFACE: {
    return InterfaceType::Bootstrap;
  }
  default: {
    return InterfaceType::Not_Recognized;
  }
  }
}

string toString(ResponseCode type) {
  switch (type) {
  case ResponseCode::Ok: {
    return "Ok";
  }
  case ResponseCode::Created: {
    return "Created";
  }
  case ResponseCode::Deleted: {
    return "Deleted";
  }
  case ResponseCode::Changed: {
    return "Changed";
  }
  case ResponseCode::Content: {
    return "Content";
  }
  case ResponseCode::Continue: {
    return "Continue";
  }
  case ResponseCode::Bad_Request: {
    return "Bad Request";
  }
  case ResponseCode::Unauthorized: {
    return "Unauthorized";
  }
  case ResponseCode::Forbidden: {
    return "Forbidden";
  }
  case ResponseCode::Not_Found: {
    return "Not Found";
  }
  case ResponseCode::Method_Not_Allowed: {
    return "Methond Not Allowed";
  }
  case ResponseCode::Not_Acceptable: {
    return "Not Acceptable";
  }
  case ResponseCode::Request_Entity_Incomplete: {
    return "Request Entity Incomplete";
  }
  case ResponseCode::Precognition_Failed: {
    return "Precognition Failed";
  }
  case ResponseCode::Request_Entity_Too_Large: {
    return "Request Entity Too Large";
  }
  case ResponseCode::Unsupported_Content_Format: {
    return "Unsupported Content Format";
  }
  case ResponseCode::Internal_Server_Error: {
    return "Internal Server Error";
  }
  case ResponseCode::Not_Implemented: {
    return "Not Implemented";
  }
  case ResponseCode::Bad_Gateway: {
    return "Bad Gateway";
  }
  case ResponseCode::Service_Unavailable: {
    return "Service Unavailable";
  }
  case ResponseCode::Gateway_Timeout: {
    return "Gateway Timeout";
  }
  case ResponseCode::Proxying_Not_Supported: {
    return "Proxying Not Supported";
  }
  case ResponseCode::Unhandled:
  default: {
    return "Unhandled Return Code";
  }
  }
}

UnsupportedResponseCode::UnsupportedResponseCode(const string& message)
    : logic_error(message) {}

string makeErrorMsg(const string& message_type,
    ResponseCode unsupported_response_code,
    const unordered_set<ResponseCode>& supported_response_codes) {
  string error_msg = message_type;
  error_msg += " does not support the use of " +
      toString(unsupported_response_code) +
      " response code! It only supports: ";
  for (auto supported_response_code : supported_response_codes) {
    error_msg += toString(supported_response_code) + "\n";
  }
  return error_msg;
}

UnsupportedResponseCode::UnsupportedResponseCode(const string& message_type,
    ResponseCode unsupported_response_code,
    const unordered_set<ResponseCode>& supported_response_codes)
    : UnsupportedResponseCode(makeErrorMsg(
          message_type, unsupported_response_code, supported_response_codes)) {}

Message::Message(EndpointPtr endpoint, // NOLINT
    MessageType message_type, InterfaceType interface,
    PayloadPtr payload, // NOLINT
    bool incoming, bool response, bool notification)
    : endpoint_(endpoint), // NOLINT
      message_type_(message_type), interface_(interface), // NOLINT
      payload_(payload), response_(response), incoming_(incoming), // NOLINT
      notification_(notification) {
  if (!endpoint_) {
    throw invalid_argument("Endpoint can not be null!");
  }
}

string Message::name() const {
  // Provide a fallback implementation
  return "Message";
}

Response::Response(EndpointPtr endpoint, // NOLINT
    MessageType message_type, InterfaceType interface, bool incoming,
    unordered_set<ResponseCode> supported_responses, // NOLINT
    ResponseCode response_code, PayloadPtr payload) // NOLINT
    : Message(endpoint, // NOLINT
          message_type, interface, payload, // NOLINT
          incoming, true),
      supported_responses_(supported_responses), // NOLINT
      response_code_(response_code) {}

void Response::checkResponseCode(ResponseCode response_code) const {
  if (supported_responses_.find(response_code) == supported_responses_.end()) {
    throw UnsupportedResponseCode(name(), response_code, supported_responses_);
  }
}

ClientRequest::ClientRequest(EndpointPtr endpoint, // NOLINT
    MessageType message_type, InterfaceType interface,
    PayloadPtr payload) // NOLINT
    : Message(endpoint, // NOLINT
          message_type, interface, payload, true) {} // NOLINT

ClientResponse::ClientResponse(EndpointPtr endpoint, // NOLINT
    ResponseCode response_code, PayloadPtr payload) // NOLINT
    : Response(endpoint, // NOLINT
          MessageType::Not_Recognized, InterfaceType::Not_Recognized, true,
          unordered_set<ResponseCode>(), response_code, payload) {} // NOLINT

ClientNotification::ClientNotification(EndpointPtr endpoint, // NOLINT
    MessageType message_type, InterfaceType interface,
    PayloadPtr payload) // NOLINT
    : Message(endpoint, // NOLINT
          message_type, interface, payload, true, false, true) {} // NOLINT

ServerRequest::ServerRequest(EndpointPtr endpoint, // NOLINT
    MessageType message_type, InterfaceType interface,
    PayloadPtr payload) // NOLINT
    : Message(endpoint, // NOLINT
          message_type, interface, payload, false) {} // NOLINT

ServerResponse::ServerResponse(EndpointPtr endpoint, // NOLINT
    MessageType message_type, InterfaceType interface,
    unordered_set<ResponseCode> supported_responses, ResponseCode response_code,
    PayloadPtr payload) // NOLINT
    : Response(endpoint, // NOLINT
          message_type, interface, false,
          supported_responses, // NOLINT
          response_code, payload) {} // NOLINT

} // namespace LwM2M

size_t std::hash<LwM2M::Message>::operator()(
    const LwM2M::Message& message) const {
  auto endoint_string = message.endpoint_->toString();
  size_t result = hash<string>{}(endoint_string);
  result <<= endoint_string.length();
  result |= static_cast<size_t>(message.message_type_);
  result <<= sizeof(message.message_type_);
  result |= static_cast<size_t>(message.interface_);
  result <<= sizeof(message.interface_);
  result |= hash<LwM2M::Payload>{}(*message.payload_);
  result <<= message.payload_->size();
  result |= message.response_; // NOLINT
  result <<= 1;
  result |= message.incoming_; // NOLINT
  result <<= 1;
  result |= message.notification_; // NOLINT
  return result;
}
