#ifndef __LWM2M_DEVICE_MANAGEMENT_WRITE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_WRITE_COMPOSITE_MESSAGE_HPP

#include "DeviceManagmentMessage.hpp"

namespace LwM2M {
/**
 * @addtogroup Message_Model
 * @{
 */

/**
 * @brief Used to selectively write Resources, and/or Resource Instances of
 * different or same Objects/Object Instances in a single request,
 *
 * Content of each write target must be formatted in application/senml+json or
 * application/senml+cbor [RFC8428] Sensor Measurement Lists data format
 *
 */
struct WriteCompositeRequest : public DeviceManagementRequest {
  WriteCompositeRequest(EndpointPtr endpoint,
      std::vector<TargetContent> content = std::vector<TargetContent>());

  std::string name() override final;
};

using WriteCompositeRequestPtr = std::shared_ptr<WriteCompositeRequest>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_WRITE_COMPOSITE_MESSAGE_HPP