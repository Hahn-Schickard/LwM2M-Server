#ifndef __LWM2M_MESSAGE_HPP
#define __LWM2M_MESSAGE_HPP

#include "DataFormat.hpp"
#include "Endpoint.hpp"

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_set>

namespace LwM2M {

struct UnsupportedOperation : public std::logic_error {
  UnsupportedOperation(std::string const &message);
};

enum struct InterfaceType : uint8_t {
  NOT_RECOGNIZED,
  REGISTRATION = 0x10,
  DEVICE_MANAGEMENT = 0x20,
  INFORMATION_REPORTING = 0x30,
  BOOTSTRAP = 0x40
};

std::string toString(InterfaceType type);

enum struct MessageType : uint8_t {
  NOT_RECOGNIZED,
  // Registration messages
  REGISTER = 0x11,
  DEREGISTER = 0x12,
  UPDATE = 0x13,
  // Device management messages
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

InterfaceType getInterfaceType(MessageType message_type);

enum struct ResponseCode : uint8_t {
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
  INTERNAL_SERVER_ERROR = 0xA0,
  NOT_IMPLEMENTED = 0xA1,
  BAD_GATEWAY = 0xA2,
  SERVICE_UNAVAILABLE = 0xA3,
  GATEWAY_TIMEOUT = 0xA4,
  PROXYING_NOT_SUPPORTED = 0xA5,
  UNHANDLED
};

std::string toString(ResponseCode type);

struct UnsupportedResponseCode : public std::logic_error {
  UnsupportedResponseCode(std::string const &message);
  UnsupportedResponseCode(
      std::string message_type, ResponseCode unsupported_response_code,
      std::unordered_set<ResponseCode> supported_response_codes);
};

struct Message {
  const EndpointPtr endpoint_;
  const MessageType message_type_;
  const InterfaceType interface_;
  const PayloadPtr payload_;
  const bool response_ = false;
  const bool incoming_ = false;
  const bool notification_ = false;

  /**
   * @brief Returns the name of derived class, used for exception throwing and
   * logging purposes
   *
   * @return std::string
   */
  virtual std::string name();

protected:
  /**
   * @brief Construct a new Message object
   * @throw std::invalid_argument if LwM2M::EndpointPtr is nullptr
   *
   * @param endpoint LwM2M::EndpointPtr
   * @param message_type LwM2M::MessageType
   * @param interface LwM2M::InterfaceType
   * @param incoming bool
   * @param response bool
   * @param notification bool
   */
  Message(EndpointPtr endpoint, MessageType message_type,
          InterfaceType interface, PayloadPtr payload, bool incoming,
          bool response = false, bool notification = false);

  virtual ~Message() = default;
};

using MessagePtr = std::shared_ptr<Message>;

struct Response : Message {
  const std::unordered_set<ResponseCode> supported_responses_;
  const ResponseCode response_code_;

protected:
  /**
   * @brief Construct a new Response object
   *
   * @throw std::invalid_argument if std::unordered_set<ResponseCode>
   * supported_responses is empty
   *
   * @param endpoint LwM2M::EndpointPtr
   * @param message_type LwM2M::MessageType
   * @param interface LwM2M::InterfaceType
   * @param incoming bool
   * @param supported_responses std::unordered_set<LwM2M::ResponseCode>
   * @param response_code LwM2M::ResponseCode
   * @param payload LwM2M::PayloadPtr
   */
  Response(EndpointPtr endpoint, MessageType message_type,
           InterfaceType interface, bool incoming,
           std::unordered_set<ResponseCode> supported_responses,
           ResponseCode response_code, PayloadPtr payload);

  /**
   * @brief Checks if a given response code is supported
   *
   * @throw LwM2M::UnsupportedResponseCode if response code is not supported
   *
   * Use this call in derived LwM2M::Response classes.
   *
   * @param response_code LwM2M::ResponseCode
   */
  void checkResponseCode(ResponseCode response_code);
};

using ResponsePtr = std::shared_ptr<Response>;

struct ClientRequest : Message {
protected:
  ClientRequest(EndpointPtr endpoint, MessageType message_type,
                InterfaceType interface, PayloadPtr payload = PayloadPtr());
};

using ClientRequestPtr = std::shared_ptr<ClientRequest>;

struct ClientResponse : Response {
  ClientResponse(EndpointPtr endpoint,
                 ResponseCode response_code = ResponseCode::BAD_REQUEST,
                 PayloadPtr payload = PayloadPtr());
};

using ClientResponsePtr = std::shared_ptr<ClientResponse>;

struct ClientNotification : Message {
protected:
  ClientNotification(EndpointPtr endpoint, MessageType message_type,
                     InterfaceType interface,
                     PayloadPtr payload = PayloadPtr());
};

using ClientNotificationPtr = std::shared_ptr<ClientNotification>;

struct ServerRequest : Message {
protected:
  ServerRequest(EndpointPtr endpoint, MessageType message_type,
                InterfaceType interface, PayloadPtr payload = PayloadPtr());
};

using ServerRequestPtr = std::shared_ptr<ServerRequest>;

struct ServerResponse : Response {
protected:
  ServerResponse(EndpointPtr endpoint, MessageType message_type,
                 InterfaceType interface,
                 std::unordered_set<ResponseCode> supported_responses,
                 ResponseCode response_code, PayloadPtr payload = PayloadPtr());
};

using ServerResponsePtr = std::shared_ptr<ServerResponse>;
} // namespace LwM2M

namespace std {
template <> struct hash<LwM2M::Message> {
  size_t operator()(const LwM2M::Message &message) const;
};
} // namespace std
#endif //__LWM2M_MESSAGE_HPP