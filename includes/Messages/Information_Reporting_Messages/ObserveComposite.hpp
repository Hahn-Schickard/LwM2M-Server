#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP

#include "Message.hpp"

#include <vector>

namespace LwM2M {
/**
 * @brief Used to start observing the given elements within the Client.
 *
 */
struct ObserveCompositeRequest : ServerRequest {
  ObserveCompositeRequest(
      EndpointPtr endpoint,
      std::vector<ElmentIdVariant> target_ids = std::vector<ElmentIdVariant>());

  std::string name() override final;
};

using ObserveCompositeRequestPtr = std::shared_ptr<ObserveCompositeRequest>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP