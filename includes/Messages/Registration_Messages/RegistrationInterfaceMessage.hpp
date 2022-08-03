#ifndef __LWM2M_REGISTRATION_INTERFACE_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @addtogroup Message_Model
 * @{
 */

struct RegistrationInterfaceError : public std::domain_error {
  ServerResponsePtr response_;
  RegistrationInterfaceError(ServerResponsePtr response)
      : domain_error("Request " + response->endpoint_->endpoint_address_ + ":" +
            std::to_string(response->endpoint_->endpoint_port_) +
            " is missing one of mandatory parameters. Sending " +
            response->name()),
        response_(response) {}
};

/**
 * @brief Generalizes all of the LwM2M Registration Interface Responses.
 *
 * @see RegisterResponse
 * @see UpdateResponse
 * @see DeregisterResponse
 *
 * All of the messages, that implement this type are Downlink (Server is sending
 * this message to the Device)
 *
 * @attention This is an abstract class of Message. It can not be built or exist
 * on its own. It must always have a concrete implementation. Accessing this
 * class without concrete implementation will cause undefined behaviour.
 */
struct RegistrationInterfaceResponse : ServerResponse {
protected:
  RegistrationInterfaceResponse(EndpointPtr endpoint, MessageType message_type,
      std::unordered_set<ResponseCode> supported_responses,
      ResponseCode response_code, PayloadPtr payload = PayloadPtr())
      : ServerResponse(endpoint, message_type, InterfaceType::REGISTRATION,
            supported_responses, response_code, payload) {}
};

/**
 * @brief Generalizes all of the LwM2M Registration Interface Requests.
 *
 * @see RegisterRequest
 * @see UpdateRequest
 * @see DeregisterRequest
 *
 * All of the messages, that implement this type are Uplink (Device is sending
 * this message to the Server)
 *
 * @attention This is an abstract class of Message. It can not be built or exist
 * on its own. It must always have a concrete implementation. Accessing this
 * class without concrete implementation will cause undefined behaviour.
 */
struct RegistrationInterfaceRequest : ClientRequest {
protected:
  RegistrationInterfaceRequest(EndpointPtr endpoint, MessageType message_type,
      PayloadPtr payload = PayloadPtr())
      : ClientRequest(
            endpoint, message_type, InterfaceType::REGISTRATION, payload) {}
};

using RegistrationInterfaceRequestPtr =
    std::shared_ptr<RegistrationInterfaceRequest>;
using RegistrationInterfaceResponsePtr =
    std::shared_ptr<RegistrationInterfaceResponse>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_MESSAGE_HPP