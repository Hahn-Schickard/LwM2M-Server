#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP

#include "InformationReporingMessage.hpp"

#include <vector>

namespace LwM2M {
/**
 * @addtogroup Message_Model
 * @{
 */

/**
 * @brief Used to start observing the given elements within the Client.
 *
 */
struct ObserveCompositeRequest : InformationReportingRequest {
  ObserveCompositeRequest(
      EndpointPtr endpoint, ElementIDs target_ids = ElementIDs());

  std::string name() const final;
};

using ObserveCompositeRequestPtr = std::shared_ptr<ObserveCompositeRequest>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP