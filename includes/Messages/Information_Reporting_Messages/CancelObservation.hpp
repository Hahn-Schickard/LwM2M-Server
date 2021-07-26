#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used stop the observation of a given element within the Client.
 *
 */
struct CancelObservationRequest : ServerRequest {
  CancelObservationRequest(EndpointPtr endpoint,
                           ObjectID target_id = ObjectID(0));

  std::string name() override final;
};

using CancelObservationRequestPtr = std::shared_ptr<CancelObservationRequest>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_MESSAGE_HPP