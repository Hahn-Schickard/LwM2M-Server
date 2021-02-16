#ifndef __LWM2M_REGISTRATION_INTERFACE_REGISTER_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_REGISTER_MESSAGE_HPP

#include "Message.hpp"
#include "ModelType.hpp"

#include <unordered_map>
#include <vector>

namespace LwM2M {
/**
 * @brief Response to LwM2M::RegisterRequest, indicates wheater the request was
 * succefull, or not
 *
 * Supported response codes:
 * - ResponseCode::CREATED - Operation was a success.
 * - ResponseCode::BAD_REQUEST - One of the mandatory parameters were not
 * specified, endpoint name does not match with CN field of X.509 Cerftificates
 * - ResponseCode::FORBIDDEN - A given endpoint name is not allowed within the
 * server.
 * - ResponseCode::PRECOGNITION_FAILED - Given LwM2M version is not supported by
 * the server.
 */
struct RegisterResponse : ServerResponse {
  RegisterResponse(EndpointPtr endpoint,
                   ResponseCode response_code = ResponseCode::BAD_REQUEST,
                   std::string location = std::string());

  std::string name() override final;
};

using RegisterResponsePtr = std::shared_ptr<RegisterResponse>;

/**
 * @brief Used to add a new device to the server
 *
 */
struct RegisterRequest : ClientRequest {
  // Mandatory fields
  const size_t life_time_;
  const LwM2M_Version version_;
  const std::unordered_map<unsigned int, std::vector<unsigned int>>
      object_instances_map_;
  // Optional fields
  const std::string endpoint_name_;
  const BindingType binding_;
  const bool queue_mode_;
  const std::string sms_number_;

  RegisterRequest(
      EndpointPtr endpoint, size_t life_time = 0,
      std::unordered_map<unsigned int, std::vector<unsigned int>>
          object_instances_map =
              std::unordered_map<unsigned int, std::vector<unsigned int>>(),
      std::string endpoint_name = std::string());

  RegisterRequest(EndpointPtr endpoint, size_t life_time, LwM2M_Version version,
                  std::unordered_map<unsigned int, std::vector<unsigned int>>
                      object_instances_map,
                  std::string endpoint_name,
                  BindingType binding = BindingType::UDP,
                  bool queue_mode = false,
                  std::string sms_number = std::string());

  std::string name() override final;

  RegisterResponsePtr
  makeResponse(ResponseCode response_code = ResponseCode::FORBIDDEN,
               std::string location = std::string());
};

using RegisterRequestPtr = std::shared_ptr<RegisterRequest>;
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_REGISTER_MESSAGE_HPP