#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_REQUEST_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_REQUEST_HPP

#include "InformationReportingInterfaceMessage.hpp"

namespace LwM2M {
struct CancelObserveComposite_Request
    : public InformationReporting_Interface_Message {
  CancelObserveComposite_Request(std::string endpoint_address,
                                 unsigned int endpoint_port,
                                 uint16_t message_id,
                                 std::vector<uint8_t> token);
};
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_REQUEST_HPP