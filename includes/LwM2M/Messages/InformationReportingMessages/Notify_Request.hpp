#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_NOTIFY_REQUEST_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_NOTIFY_REQUEST_HPP

#include "InformationReportingInterfaceMessage.hpp"

namespace LwM2M {
struct Notify_Request : public InformationReporting_Interface_Message {
  std::string value_;

  Notify_Request(std::string endpoint_address, unsigned int endpoint_port,
                 uint16_t message_id, std::vector<uint8_t> token,
                 std::string value);
};

} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_NOTIFY_REQUEST_HPP