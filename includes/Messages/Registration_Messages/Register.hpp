#ifndef __LWM2M_REGISTRATION_INTERFACE_REGISTER_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_REGISTER_MESSAGE_HPP

#include "Message.hpp"
#include "ModelType.hpp"

#include <optional>
#include <unordered_map>
#include <vector>

namespace LwM2M {
/**
 * @brief Response to LwM2M::RegisterRequest, indicates if the request was
 * successfull, or not
 *
 */
struct RegisterResponse : ServerResponse {
  /**
   * @brief Creates a RegisterResponse with one of the supported failure codes.
   *
   * Used to indicate failure.
   *
   * Supported response codes:
   * - ResponseCode::BAD_REQUEST - One of the mandatory parameters were not
   * specified, endpoint name does not match with CN field of X.509 Certificates
   * - ResponseCode::FORBIDDEN - A given endpoint name is not allowed within the
   * server.
   * - ResponseCode::PRECOGNITION_FAILED - Given LwM2M version is not supported
   * by the server.
   *
   * @param endpoint
   * @param response_code
   */
  RegisterResponse(EndpointPtr endpoint, ResponseCode response_code);

  /**
   * @brief Creates a RegisterResponse with assigned endpoint location.
   *
   * Used to indicate success.
   *
   * @param endpoint
   * @param location
   */
  RegisterResponse(EndpointPtr endpoint, std::string location);

  RegisterResponse(EndpointPtr endpoint);

  std::string name() override final;
};

using RegisterResponsePtr = std::shared_ptr<RegisterResponse>;

/**
 * @brief Used to add a new device to the server
 *
 */
struct RegisterRequest : ClientRequest {
  const size_t life_time_;
  const std::unordered_map<unsigned int, std::vector<unsigned int>>
      object_instances_map_;
  const std::optional<std::string> endpoint_name_;
  const LwM2M_Version version_;
  const std::optional<BindingType> binding_;
  const std::optional<bool> queue_mode_;
  const std::optional<std::string> sms_number_;

  RegisterRequest(
      EndpointPtr endpoint, size_t life_time = 0,
      std::unordered_map<unsigned int, std::vector<unsigned int>>
          object_instances_map =
              std::unordered_map<unsigned int, std::vector<unsigned int>>(),
      std::optional<std::string> endpoint_name = std::nullopt,
      LwM2M_Version version = LwM2M_Version::V1_0,
      std::optional<BindingType> binding = std::nullopt,
      std::optional<bool> queue_mode = std::nullopt,
      std::optional<std::string> sms_number = std::nullopt);

  std::string name() override final;

  /**
   * @brief Creates a response that indicates failure.
   *
   * @param response_code
   * @return RegisterResponsePtr
   */
  RegisterResponsePtr makeResponse(ResponseCode response_code);

  /**
   * @brief Creates a response that indicates success.
   *
   * @param location
   * @return RegisterResponsePtr
   */
  RegisterResponsePtr makeResponse(std::string location);
};

using RegisterRequestPtr = std::shared_ptr<RegisterRequest>;
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_REGISTER_MESSAGE_HPP