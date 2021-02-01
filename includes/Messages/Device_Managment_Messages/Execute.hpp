#ifndef __LWM2M_DEVICE_MANAGMENT_EXECUTE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_EXECUTE_MESSAGE_HPP

#include "Message.hpp"

#include <vector>

namespace LwM2M {
/**
 * @brief Used to initiate some action, and can only be
 * performed on individual Resources.
 *
 * Arguments MUST be formated in text/plain
 * plain text, application/senml+json or application/senml+cbor Sensor
 * Measurement Lists data formats
 *
 */
struct ExecuteRequest : ServerRequest {
  const ResourceID target_;
  const std::vector<DataFormatPtr> arguments_;

  ExecuteRequest(EndpointPtr endpoint, ResourceID target);
  ExecuteRequest(EndpointPtr endpoint, ResourceID target,
                 std::vector<DataFormatPtr> arguments);

  std::string name() override final;
};

using ExecuteRequestPtr = std::shared_ptr<ExecuteRequest>;

/**
 * @brief Response to LwM2M::ExecuteRequest, indicates wheater the request was
 * succefull, requires more data or failed
 *
 * Supported response codes:
 * ResponseCode::CHANGED
 * ResponseCode::BAD_REQUEST
 * ResponseCode::UNAUTHORIZED
 * ResponseCode::NOT_FOUND
 * ResponseCode::METHOD_NOT_ALLOWED
 */
struct ExecuteResponse : ClientResponse {
  ExecuteResponse(EndpointPtr endpoint, ResponseCode response_code);

  std::string name() override final;
};

} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_EXECUTE_MESSAGE_HPP