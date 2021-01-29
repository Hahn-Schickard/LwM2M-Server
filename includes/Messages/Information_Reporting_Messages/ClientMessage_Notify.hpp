#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_CLIENT_MESSAGE_NOTIFY_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_CLIENT_MESSAGE_NOTIFY_HPP

#include "InformationReportingMessage.hpp"

namespace LwM2M {
struct ClientMessage_Notify : public InformationReportingMessage {
  std::string value_;

  ClientMessage_Notify(std::string endpoint_address, unsigned int endpoint_port,
                       uint16_t message_id, std::vector<uint8_t> token,
                       std::string value);
};

} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_CLIENT_MESSAGE_NOTIFY_HPP