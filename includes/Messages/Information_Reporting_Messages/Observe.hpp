#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to start observing a given element within the client.
 *
 */
struct ObserveRequest : ServerRequest {
  ObserveRequest(EndpointPtr endpoint, ObjectID target_id);

  std::string name() override final;
};

using ObserveRequestPtr = std::shared_ptr<ObserveRequest>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP