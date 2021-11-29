#ifndef __LWM2M_DEVICE_MANAGEMENT_DISCOVER_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_DISCOVER_MESSAGE_HPP

#include "DeviceManagmentMessage.hpp"

namespace LwM2M {
/**
 * @brief Used to discover LwM2M Attributes attached to an Object, Object
 * Instances, and Resources as well as what Resources are instantiated in a
 * given Object Instance.
 *
 */
struct DiscoverRequest : DeviceManagementRequest {
  DiscoverRequest(EndpointPtr endpoint, ElementID target_id = ElementID(0));

  std::string name() override final;

  ElementID target_;
};

using DiscoverRequestPtr = std::shared_ptr<DiscoverRequest>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_DISCOVER_MESSAGE_HPP