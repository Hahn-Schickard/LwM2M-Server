#ifndef __LWM2M_DEVICE_MANAGEMENT_EXECUTE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_EXECUTE_MESSAGE_HPP

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
  ExecuteRequest(EndpointPtr endpoint, ResourceID target = ResourceID(0, 0, 0));
  ExecuteRequest(EndpointPtr endpoint, ResourceID target,
                 std::string arguments);

  std::string name() override final;
};

using ExecuteRequestPtr = std::shared_ptr<ExecuteRequest>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_EXECUTE_MESSAGE_HPP