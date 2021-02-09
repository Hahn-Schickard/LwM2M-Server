#ifndef __LWM2M_REGISTRATION_INTERFACE_UPDATE_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_UPDATE_MESSAGE_HPP

#include "Message.hpp"
#include "ModelType.hpp"

#include <optional>
#include <unordered_map>
#include <vector>

namespace LwM2M {

/**
 * @brief Response to LwM2M::UpdateRequest, indicates wheater the request was
 * succefull, or not
 *
 * Supported response codes:
 * - ResponseCode::CHANGED - Operation was a success.
 * - ResponseCode::BAD_REQUEST - No location was given in the request.
 * - ResponseCode::NOT_FOUND - Given location could not be found in the server
 */
struct UpdateResponse : ServerResponse {
  UpdateResponse(EndpointPtr endpoint,
                 ResponseCode response_code = ResponseCode::BAD_REQUEST);

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
      EndpointPtr endpoint, std::string location = std::string(),
      std::unordered_map<unsigned int, std::vector<unsigned int>>
          object_instances_map =
              std::unordered_map<unsigned int, std::vector<unsigned int>>(),
      std::optional<size_t> lifetime = std::nullopt,
      std::optional<BindingType> binding = std::nullopt,
      std::optional<std::string> sms_number = std::nullopt);

  std::string name() override final;

  UpdateResponsePtr
  makeResponse(ResponseCode response_code = ResponseCode::BAD_REQUEST);
};

using UpdateRequestPtr = std::shared_ptr<UpdateRequest>;

} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_UPDATE_MESSAGE_HPP