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
/**
 * @defgroup Message_Model Message Model
 * Contains All possible interaction definitions between the Server and the
 * Client
 * @addtogroup Message_Model
 * @{
 */

struct UnsupportedOperation : public std::logic_error {
  UnsupportedOperation(const std::string& message);
};

enum struct InterfaceType : uint8_t {
  Not_Recognized,
  Registration = 0x10,
  Device_Management = 0x20,
  Information_Reporting = 0x30,
  Bootstrap = 0x40
};

std::string toString(InterfaceType type);

enum struct MessageType : uint8_t {
  Not_Recognized,
  // Registration messages
  Register = 0x11,
  Deregister = 0x12,
  Update = 0x13,
  // Device management messages
  Read = 0x21,
  Write = 0x22,
  Execute = 0x23,
  Create = 0x24,
  Delete = 0x25,
  Write_Attributes = 0x26,
  Discover = 0x27,
  Read_Composite = 0x28,
  Write_Composite = 0x29,
  // Information reporting messages
  Observe = 0x30,
  Observe_Composite = 0x31,
  Cancel_Observation = 0x32,
  Cancel_Observation_Composite = 0x33,
  Notify = 0x34,
  Send = 0x35
};

std::string toString(MessageType type);

InterfaceType getInterfaceType(MessageType message_type);

enum struct ResponseCode : uint8_t {
  Ok = 0x40,
  Created = 0x41,
  Deleted = 0x42,
  Changed = 0x44,
  Content = 0x45,
  Continue = 0x5F,
  Bad_Request = 0x80,
  Unauthorized = 0x81,
  Forbidden = 0x83,
  Not_Found = 0x84,
  Method_Not_Allowed = 0x85,
  Not_Acceptable = 0x86,
  Request_Entity_Incomplete = 0x88,
  Precognition_Failed = 0x8C,
  Request_Entity_Too_Large = 0x8D,
  Unsupported_Content_Format = 0x8F,
  Internal_Server_Error = 0xA0,
  Not_Implemented = 0xA1,
  Bad_Gateway = 0xA2,
  Service_Unavailable = 0xA3,
  Gateway_Timeout = 0xA4,
  Proxying_Not_Supported = 0xA5,
  Unhandled
};

std::string toString(ResponseCode type);

struct UnsupportedResponseCode : public std::logic_error {
  UnsupportedResponseCode(const std::string& message);
  UnsupportedResponseCode(const std::string& message_type,
      ResponseCode unsupported_response_code,
      const std::unordered_set<ResponseCode>& supported_response_codes);
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
  virtual std::string name() const;

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
  void checkResponseCode(ResponseCode response_code) const;
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
      ResponseCode response_code = ResponseCode::Bad_Request,
      PayloadPtr payload = PayloadPtr());
};

using ClientResponsePtr = std::shared_ptr<ClientResponse>;

struct ClientNotification : Message {
protected:
  ClientNotification(EndpointPtr endpoint, MessageType message_type,
      InterfaceType interface, PayloadPtr payload = PayloadPtr());
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

/** @}*/
} // namespace LwM2M

namespace std {
/**
 * @ingroup  Message_Model
 */
template <> struct hash<LwM2M::Message> {
  size_t operator()(const LwM2M::Message& message) const;
};
} // namespace std
#endif //__LWM2M_MESSAGE_HPP