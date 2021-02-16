#ifndef __LWM2M_DEVICE_MANAGMENT_WRITE_ATTRIBUTE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_WRITE_ATTRIBUTE_MESSAGE_HPP

#include "Message.hpp"

#include <vector>

namespace LwM2M {

/**
 * @brief Used to modify multiple Attributes of Objects/Object
 * Instances/Resoruces/Resource Instances in a single operation
 *
 */
struct WriteAttributesRequest : ServerRequest {
  WriteAttributesRequest(EndpointPtr endpoint);
  WriteAttributesRequest(EndpointPtr endpoint,
                         std::vector<TargetAttribute> content);
  WriteAttributesRequest(EndpointPtr endpoint,
                         std::vector<ElmentIdVariant> targets,
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
 * - ResponseCode::NOT_FOUND - None of target LwM2M::ElmentIdVariant instances
 * point to a valid element within the client.
 * - ResponseCode::METHOD_NOT_ALLOWED - No target LwM2M::ElmentIdVariant
 * instances are allowed to use WriteAttributes operation.
 */
struct WriteAttributesResponse : ClientResponse {
  WriteAttributesResponse(EndpointPtr endpoint, ResponseCode response_code =
                                                    ResponseCode::BAD_REQUEST);

  std::string name() override final;
};

using WriteAttributesResponsePtr = std::shared_ptr<WriteAttributesResponse>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_WRITE_ATTRIBUTE_MESSAGE_HPP