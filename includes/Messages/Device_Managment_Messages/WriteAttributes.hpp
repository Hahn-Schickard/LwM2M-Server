#ifndef __LWM2M_DEVICE_MANAGMENT_WRITE_ATTRIBUTE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_WRITE_ATTRIBUTE_MESSAGE_HPP

#include "Message.hpp"

#include <optional>
#include <vector>

namespace LwM2M {

struct NotifyAttribute {
  const std::optional<unsigned int> minimum_period_;
  const std::optional<unsigned int> maximum_period_;
  const std::optional<unsigned int> greater_than_;
  const std::optional<unsigned int> less_than_;
  const std::optional<unsigned int> step_;
  const std::optional<unsigned int> minimum_evaluation_period_;
  const std::optional<unsigned int> maximum_evaluation_period_;

  NotifyAttribute(
      std::optional<unsigned int> minimum_period = std::nullopt,
      std::optional<unsigned int> maximum_period = std::nullopt,
      std::optional<unsigned int> greater_than = std::nullopt,
      std::optional<unsigned int> less_than = std::nullopt,
      std::optional<unsigned int> step = std::nullopt,
      std::optional<unsigned int> minimum_evaluation_period = std::nullopt,
      std::optional<unsigned int> maximum_evaluation_period = std::nullopt)
      : minimum_period_(minimum_period), maximum_period_(maximum_period),
        greater_than_(greater_than), less_than_(less_than), step_(step),
        minimum_evaluation_period_(minimum_evaluation_period),
        maximum_evaluation_period_(maximum_evaluation_period) {}
};

using NotifyAttributePtr = std::shared_ptr<NotifyAttribute>;
using TargetAttribute = std::pair<EelmentIdVariant, NotifyAttributePtr>;

/**
 * @brief Used to modify multiple Attributes of Objects/Object
 * Instances/Resoruces/Resource Instances in a single operation
 *
 */
struct WriteAttributesRequest : ServerRequest {
  std::vector<TargetAttribute> content_;

  WriteAttributesRequest(EndpointPtr endpoint);
  WriteAttributesRequest(EndpointPtr endpoint,
                         std::vector<TargetAttribute> content);
  WriteAttributesRequest(EndpointPtr endpoint,
                         std::vector<EelmentIdVariant> targets,
                         NotifyAttributePtr attribute);

  void append(EelmentIdVariant target, NotifyAttributePtr attribute);
  void append(std::vector<EelmentIdVariant> targets,
              NotifyAttributePtr attribute);

  std::string name() override final;
};

/**
 * @brief Response to LwM2M::WriteAttributesRequest, indicates wheater the
 * request was succefull, or not
 *
 * Supported response codes:
 * - ResponseCode::CHANGED - Operation was a success.
 * - ResponseCode::BAD_REQUEST - Data format is written in wrong Content Format.
 * - ResponseCode::UNAUTHORIZED - Access rights permission denied.
 * - ResponseCode::NOT_FOUND - None of target EelmentIdVariant instances point
 * to a valid element within the client.
 * - ResponseCode::METHOD_NOT_ALLOWED - No target EelmentIdVariant instances are
 * allowed to use WriteAttributes operation.
 */
struct WriteAttributesResponse : ClientResponse {
  WriteAttributesResponse(EndpointPtr endpoint, ResponseCode response_code =
                                                    ResponseCode::BAD_REQUEST);

  std::string name() override final;
};

using WriteAttributesResponsePtr = std::shared_ptr<WriteAttributesResponse>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_WRITE_ATTRIBUTE_MESSAGE_HPP