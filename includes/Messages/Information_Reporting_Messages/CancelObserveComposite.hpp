#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP

#include "InformationReporingMessage.hpp"

namespace LwM2M {
/**
 * @addtogroup Message_Model
 * @{
 */

/**
 * @brief Used stop the observation of a given elements within the Client.
 *
 */
struct CancelObserveCompositeRequest : InformationReportingRequest {
  CancelObserveCompositeRequest(
      EndpointPtr endpoint, ElementIDs target_ids = ElementIDs());

  std::string name() override final;
};

using CancelObserveCompositeRequestPtr =
    std::shared_ptr<CancelObserveCompositeRequest>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP