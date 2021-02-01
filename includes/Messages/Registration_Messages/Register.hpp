#ifndef __LWM2M_REGISTRATION_INTERFACE_REGISTER_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_REGISTER_MESSAGE_HPP

#include "Message.hpp"
#include "ModelType.hpp"

#include <unordered_map>
#include <vector>

namespace LwM2M {
struct RegisterResponse : ServerResponse {
  // Mandatory fields
  const std::string location_;

  RegisterResponse(EndpointPtr endpoint, ResponseCode response_code,
                   std::string location)
      : ServerResponse(
            endpoint, MessageType::REGISTER, InterfaceType::REGISTRATION,
            std::unordered_set<ResponseCode>{
                ResponseCode::CREATED, ResponseCode::BAD_REQUEST,
                ResponseCode::FORBIDDEN, ResponseCode::PRECOGNITION_FAILED},
            response_code),
        location_(location) {
    checkResponseCode(response_code);
  }

  std::string name() override final { return "RegisterResponse"; }
};

using RegisterResponsePtr = std::shared_ptr<RegisterResponse>;

struct RegisterRequest : ClientRequest {
  // Mandatory fields
  const size_t life_time_;
  const LwM2M_Version version_;
  const std::unordered_map<unsigned int, std::vector<unsigned int>>
      object_instances_map_;
  // Optional fields
  const std::string endpoint_name_ = std::string();
  const BindingType binding_ = BindingType::UDP;
  const bool queue_mode_ = false;
  const std::string sms_number_ = std::string();

  RegisterRequest(EndpointPtr endpoint, size_t life_time, LwM2M_Version version,
                  std::unordered_map<unsigned int, std::vector<unsigned int>>
                      object_instances_map)
      : ClientRequest(endpoint, MessageType::REGISTER,
                      InterfaceType::REGISTRATION),
        life_time_(life_time), version_(version),
        object_instances_map_(object_instances_map) {}

  std::string name() override final { return "RegisterRequest"; }

  RegisterResponsePtr
  makeResponse(ResponseCode response_code = ResponseCode::FORBIDDEN,
               std::string location = std::string()) {
    return std::make_shared<RegisterResponse>(endpoint_, response_code,
                                              location);
  }
};

using RegisterRequestPtr = std::shared_ptr<RegisterRequest>;
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_REGISTER_MESSAGE_HPP