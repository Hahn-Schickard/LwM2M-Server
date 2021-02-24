#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used stop the observation of a given elements within the Client.
 *
 */
struct CancelObserveCompositeRequest : ServerRequest {
  CancelObserveCompositeRequest(
      EndpointPtr endpoint,
      std::vector<ElmentIdVariant> target_ids = std::vector<ElmentIdVariant>());

  std::string name() override final;
};

using CancelObserveCompositeRequestPtr =
    std::shared_ptr<CancelObserveCompositeRequest>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP