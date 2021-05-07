#ifndef __LWM2M_DEVICE_MANAGEMENT_READ_COMPOSITE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_READ_COMPOSITE_MESSAGE_HPP

#include "Message.hpp"

#include <vector>

namespace LwM2M {
/**
 * @brief Used to selectively read any combination of Objects,Object
 * Instance(s), Resources, and/or Resource Instances of different or same
 * Objects/Object Instances in a single request
 *
 */
struct ReadCompositeRequest : ServerRequest {
  /**
   * @brief Construct a new Read Composite Request object
   *
   * @param endpoint
   * @param uri_targets - a list of Object/Object Instance/Resource/Resource
   * Instance URIs that will be read
   */
  ReadCompositeRequest(
      EndpointPtr endpoint,
      std::vector<ElmentIdVariant> targets = std::vector<ElmentIdVariant>());

  std::string name() override final;
};

using ReadCompositeRequestPtr = std::shared_ptr<ReadCompositeRequest>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_READ_COMPOSITE_MESSAGE_HPP