#ifndef __LWM2M_DEVICE_MANAGEMENT_READ_COMPOSITE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_READ_COMPOSITE_MESSAGE_HPP

#include "DeviceManagmentMessage.hpp"

#include <vector>

namespace LwM2M {
/**
 * @addtogroup Message_Model
 * @{
 */

/**
 * @brief Used to selectively read any combination of Objects,Object
 * Instance(s), Resources, and/or Resource Instances of different or same
 * Objects/Object Instances in a single request
 *
 */
struct ReadCompositeRequest : DeviceManagementRequest {
  /**
   * @brief Construct a new Read Composite Request object
   *
   * @param endpoint
   * @param targets - a list of Object/Object Instance/Resource/Resource
   * Instance URIs that will be read
   */
  ReadCompositeRequest(EndpointPtr endpoint, ElementIDs targets = ElementIDs());

  std::string name() override final;
};

using ReadCompositeRequestPtr = std::shared_ptr<ReadCompositeRequest>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_READ_COMPOSITE_MESSAGE_HPP