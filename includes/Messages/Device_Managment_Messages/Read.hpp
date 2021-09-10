#ifndef __LWM2M_DEVICE_MANAGEMENT_READ_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_READ_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to access the value of a Resource, a Resource Instance, an array
 * of Resource Instances, anObject Instance or all the Object Instances of an
 * Object.
 *
 */
struct ReadRequest : ServerRequest {
  ReadRequest(EndpointPtr endpoint, ElementID target_id = ElementID(0));

  std::string name() override final;

  ElementID target_;
};

using ReadRequestPtr = std::shared_ptr<ReadRequest>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_READ_MESSAGE_HPP