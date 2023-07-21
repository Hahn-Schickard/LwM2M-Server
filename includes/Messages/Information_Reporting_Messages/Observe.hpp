#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP

#include "InformationReporingMessage.hpp"

namespace LwM2M {
/**
 * @addtogroup Message_Model
 * @{
 */

/**
 * @brief Used to start observing a given element within the client.
 *
 */
struct ObserveRequest : InformationReportingRequest {
  ObserveRequest(EndpointPtr endpoint, ElementID target_id = ElementID(0));

  std::string name() const final;
};

using ObserveRequestPtr = std::shared_ptr<ObserveRequest>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP