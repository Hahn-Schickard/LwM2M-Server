#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_VALUE_UPDATED_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_VALUE_UPDATED_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Issued by the client to the server, if a valid ObserveRequest or
 * ObserveCompositeRequest was sent out. Contains changed value of observed
 * element.
 *
 */
struct ValueUpdated : ClientNotification {
  const DataFormatPtr content_;

  ValueUpdated(EndpointPtr endpoint, DataFormatPtr content = DataFormatPtr());

  std::string name() override final;
};

using ValueUpdatedPtr = std::shared_ptr<ValueUpdated>;

} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_VALUE_UPDATED_MESSAGE_HPP