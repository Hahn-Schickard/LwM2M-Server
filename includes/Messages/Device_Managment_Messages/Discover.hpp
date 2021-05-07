#ifndef __LWM2M_DEVICE_MANAGEMENT_DISCOVER_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_DISCOVER_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to discover LwM2M Attributes attached to an Object, Object
 * Instances, and Resources as well as what Resources are instantiated in a
 * given Object Instance.
 *
 */
struct DiscoverRequest : ServerRequest {
  DiscoverRequest(EndpointPtr endpoint,
                  ElmentIdVariant target_id = ObjectID(0));

  std::string name() override final;
};

using DiscoverRequestPtr = std::shared_ptr<DiscoverRequest>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_DISCOVER_MESSAGE_HPP