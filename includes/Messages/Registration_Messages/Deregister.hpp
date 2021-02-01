#ifndef __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
struct DeregisterResponse : ServerResponse {
  DeregisterResponse(EndpointPtr endpoint, ResponseCode response_code)
      : ServerResponse(endpoint, MessageType::DEREGISTER,
                       InterfaceType::REGISTRATION,
                       std::unordered_set<ResponseCode>{
                           ResponseCode::DELETED, ResponseCode::BAD_REQUEST,
                           ResponseCode::NOT_FOUND},
                       response_code) {
    checkResponseCode(response_code);
  }

  std::string name() override final { return "DeregisterResponse"; }
};

using DeregisterResponsePtr = std::shared_ptr<DeregisterResponse>;

struct DeregisterRequest : ClientRequest {
  const std::string location_;

  DeregisterRequest(EndpointPtr endpoint, std::string location)
      : ClientRequest(endpoint, MessageType::DEREGISTER,
                      InterfaceType::REGISTRATION),
        location_(location) {}

  std::string name() override final { return "DeregisterRequest"; }

  DeregisterResponsePtr makeResponse(ResponseCode response_code) {
    return std::make_shared<DeregisterResponse>(endpoint_, response_code);
  }
};

using DeregisterRequestPtr = std::shared_ptr<DeregisterRequest>;
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_DEREGISTER_MESSAGE_HPP