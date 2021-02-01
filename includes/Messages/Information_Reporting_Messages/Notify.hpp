#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_VALUE_UPDATED_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_VALUE_UPDATED_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
struct ValueUpdated : ClientNotification {
  const DataFormatPtr content_;

  ValueUpdated(EndpointPtr endpoint, DataFormatPtr content)
      : ClientNotification(endpoint, MessageType::NOTIFY,
                           InterfaceType::INFORMATION_REPORTING),
        content_(content) {}

  std::string name() override final { return "ValueUpdated"; }
};

using ValueUpdatedPtr = std::shared_ptr<ValueUpdated>;

} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_VALUE_UPDATED_MESSAGE_HPP