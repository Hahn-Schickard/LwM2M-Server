#ifndef __LWM2M_REGISTRATION_INTERFACE_UPDATE_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_UPDATE_MESSAGE_HPP

#include "Message.hpp"
#include "ModelType.hpp"

#include <optional>
#include <unordered_map>
#include <vector>

namespace LwM2M {

/**
 * @brief Response to LwM2M::UpdateRequest, indicates if the request was
 * successfull, or not
 *
 */
struct UpdateResponse : ServerResponse {
  /**
   * @brief Creates a UpdateResponse with one of the supported failure codes.
   *
   * Used to indicate failure.
   *
   * Supported response codes:
   * - ResponseCode::BAD_REQUEST - No location was given in the request.
   * - ResponseCode::NOT_FOUND - Given location could not be found in the server
   *
   * @param endpoint
   * @param response_code
   */
  UpdateResponse(EndpointPtr endpoint, ResponseCode response_code);

  /**
   * @brief Creates a UpdateResponse with assigned endpoint location.
   *
   * Used to indicate success.
   *
   * @param endpoint
   * @param location
   */
  UpdateResponse(EndpointPtr endpoint, std::string location);

  UpdateResponse(EndpointPtr endpoint);

  std::string name() override final;
};

using UpdateResponsePtr = std::shared_ptr<UpdateResponse>;

/**
 * @brief Used to update certain LwM2M::Device fields
 *
 */
struct UpdateRequest : ClientRequest {
  const std::string location_;
  const std::unordered_map<unsigned int, std::vector<unsigned int>>
      object_instances_map_;
  const std::optional<size_t> lifetime_;
  const std::optional<BindingType> binding_;
  const std::optional<std::string> sms_number_;

  UpdateRequest(
      EndpointPtr endpoint, std::string location,
      std::unordered_map<unsigned int, std::vector<unsigned int>>
          object_instances_map =
              std::unordered_map<unsigned int, std::vector<unsigned int>>(),
      std::optional<size_t> lifetime = std::nullopt,
      std::optional<BindingType> binding = std::nullopt,
      std::optional<std::string> sms_number = std::nullopt);

  std::string name() override final;

  UpdateResponsePtr makeResponse(ResponseCode response_code);
};

using UpdateRequestPtr = std::shared_ptr<UpdateRequest>;

} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_UPDATE_MESSAGE_HPP