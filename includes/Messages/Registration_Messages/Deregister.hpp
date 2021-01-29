#ifndef __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP

#include "RegistrationInterfaceMessage.hpp"

namespace LwM2M {
struct DeregisterResponse : RegirstrationInterfaceResponse {
  DeregisterResponse(EndpointPtr endpoint, ResponseCode response_code)
      : RegirstrationInterfaceResponse(
            endpoint, MessageType::DEREGISTER,
            std::unordered_set<ResponseCode>{ResponseCode::DELETED,
                                             ResponseCode::BAD_REQUEST,
                                             ResponseCode::NOT_FOUND},
            response_code) {
    checkResponseCode(response_code);
  }

  std::string name() override final { return "DeregisterResponse"; }
};

using DeregisterResponsePtr = std::shared_ptr<DeregisterResponse>;

struct DeregisterRequest : RegirstrationInterfaceRequest {
  const std::string location_;

  DeregisterRequest(EndpointPtr endpoint, std::string location)
      : RegirstrationInterfaceRequest(endpoint, MessageType::DEREGISTER),
        location_(location) {}

  std::string name() override final { return "DeregisterRequest"; }

  DeregisterResponsePtr makeResponse(ResponseCode response_code) {
    return std::make_shared<DeregisterResponse>(endpoint_, response_code);
  }
};

using DeregisterRequestPtr = std::shared_ptr<DeregisterRequest>;
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP