#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_SERVER_REQUEST_OBSERVE_COMPOSITE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_SERVER_REQUEST_OBSERVE_COMPOSITE_HPP

#include "InformationReportingInterfaceMessage.hpp"

namespace LwM2M {
struct ServerRequest_ObserveComposite
    : public InformationReporting_Interface_Message {
  ServerRequest_ObserveComposite(std::string endpoint_address,
                                 unsigned int endpoint_port,
                                 uint16_t message_id,
                                 std::vector<uint8_t> token);
};
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_SERVER_REQUEST_OBSERVE_COMPOSITE_HPP