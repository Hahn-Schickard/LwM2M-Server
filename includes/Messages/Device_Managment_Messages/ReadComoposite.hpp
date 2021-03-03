#ifndef __LWM2M_DEVICE_MANAGMENT_READ_COMPOSITE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_READ_COMPOSITE_MESSAGE_HPP

#include "Message.hpp"

#include <vector>

namespace LwM2M {
/**
 * @brief Used to selectively read any combination of Objects,Object
 * Instance(s), Resources, and/or Resource Instances of different or same
 * Objects/Object Instances in a single request
 *
 */
struct ReadComopositeRequest : ServerRequest {
  /**
   * @brief Construct a new Read Comoposite Request object
   *
   * @param endpoint
   * @param uri_targets - a list of Object/Object Instance/Resource/Resoruce
   * Instance URIs that will be read
   */
  ReadComopositeRequest(
      EndpointPtr endpoint,
      std::vector<ElmentIdVariant> targets = std::vector<ElmentIdVariant>());

  std::string name() override final;
};

using ReadComopositeRequestPtr = std::shared_ptr<ReadComopositeRequest>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_READ_COMPOSITE_MESSAGE_HPP