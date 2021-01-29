#ifndef __LWM2M_REGISTRATION_INTERFACE_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
struct RegirstrationInterfaceRequest : ClientRequest {
  RegirstrationInterfaceRequest(EndpointPtr endpoint, MessageType message_type)
      : ClientRequest(endpoint, message_type, InterfaceType::REGISTRATION) {}
};

using RegirstrationInterfaceRequestPtr =
    std::shared_ptr<RegirstrationInterfaceRequest>;

struct RegirstrationInterfaceResponse : ServerResponse {
  RegirstrationInterfaceResponse(
      EndpointPtr endpoint, MessageType message_type,
      std::unordered_set<ResponseCode> supported_responses,
      ResponseCode response_code)
      : ServerResponse(endpoint, message_type, InterfaceType::REGISTRATION,
                       supported_responses, response_code) {}
};

using RegirstrationInterfaceResponsePtr =
    std::shared_ptr<RegirstrationInterfaceResponse>;

}; // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_MESSAGE_HPP