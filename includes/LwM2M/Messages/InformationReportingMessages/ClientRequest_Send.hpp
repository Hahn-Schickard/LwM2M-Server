#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_CLIENT_REQUEST_SEND_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_CLIENT_REQUEST_SEND_HPP

#include "InformationReportingMessage.hpp"

namespace LwM2M {
struct ClientRequest_Send : public InformationReportingMessage {
  ClientRequest_Send(std::string endpoint_address, unsigned int endpoint_port,
                     uint16_t message_id, std::vector<uint8_t> token);
};
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_CLIENT_REQUEST_SEND_HPP