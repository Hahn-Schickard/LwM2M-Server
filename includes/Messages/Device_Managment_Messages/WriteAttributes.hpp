#ifndef __LWM2M_DEVICE_MANAGEMENT_WRITE_ATTRIBUTE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_WRITE_ATTRIBUTE_MESSAGE_HPP

#include "DeviceManagmentMessage.hpp"

#include <vector>

namespace LwM2M {

/**
 * @brief Used to modify multiple Attributes of Objects/Object
 * Instances/Resources/Resource Instances in a single operation
 *
 */
struct WriteAttributesRequest : DeviceManagementRequest {
  WriteAttributesRequest(EndpointPtr endpoint);
  WriteAttributesRequest(EndpointPtr endpoint,
                         std::vector<TargetAttribute> content);
  WriteAttributesRequest(EndpointPtr endpoint, ElementIDs targets,
                         NotifyAttributePtr attribute);

  std::string name() override final;
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_WRITE_ATTRIBUTE_MESSAGE_HPP