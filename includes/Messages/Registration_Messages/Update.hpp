#ifndef __LWM2M_REGISTRATION_INTERFACE_UPDATE_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_UPDATE_MESSAGE_HPP

#include "Message.hpp"
#include "ModelType.hpp"

#include <optional>
#include <unordered_map>
#include <vector>

namespace LwM2M {

struct UpdateResponse : ServerResponse {
  UpdateResponse(EndpointPtr endpoint, ResponseCode response_code)
      : ServerResponse(endpoint, MessageType::UPDATE,
                       InterfaceType::REGISTRATION,
                       std::unordered_set<ResponseCode>{
                           ResponseCode::CHANGED, ResponseCode::BAD_REQUEST,
                           ResponseCode::NOT_FOUND},
                       response_code) {
    checkResponseCode(response_code);
  }

  std::string name() override final { return "UpdateResponse"; }
};

using UpdateResponsePtr = std::shared_ptr<UpdateResponse>;

struct UpdateRequest : ClientRequest {
  const std::string location_;
  const std::unordered_map<unsigned int, std::vector<unsigned int>>
      object_instances_map_;
  const std::optional<size_t> lifetime_;
  const std::optional<BindingType> binding_;
  const std::optional<std::string> sms_number_;

  UpdateRequest(EndpointPtr endpoint, std::string location,
                std::unordered_map<unsigned int, std::vector<unsigned int>>
                    object_instances_map,
                std::optional<size_t> lifetime = std::nullopt,
                std::optional<BindingType> binding = std::nullopt,
                std::optional<std::string> sms_number = std::nullopt)
      : ClientRequest(endpoint, MessageType::UPDATE,
                      InterfaceType::REGISTRATION),
        location_(location), object_instances_map_(object_instances_map),
        lifetime_(lifetime), binding_(binding), sms_number_(sms_number) {}

  std::string name() override final { return "UpdateRequest"; }

  UpdateResponsePtr makeResponse(ResponseCode response_code) {
    return std::make_shared<UpdateResponse>(endpoint_, response_code);
  }
};

using UpdateRequestPtr = std::shared_ptr<UpdateRequest>;

} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_UPDATE_MESSAGE_HPP