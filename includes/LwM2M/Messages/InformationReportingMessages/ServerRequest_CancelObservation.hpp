#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_SERVER_REQUES_CANCEL_OBSERVE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_SERVER_REQUES_CANCEL_OBSERVE_HPP

#include "InformationReportingInterfaceMessage.hpp"

namespace LwM2M {
struct ServerRequest_CancelObservation
    : public InformationReporting_Interface_Message {
  std::string location_;

  ServerRequest_CancelObservation(std::string endpoint_address,
                                  unsigned int endpoint_port,
                                  uint16_t message_id,
                                  std::vector<uint8_t> token,
                                  std::string location);
};
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_SERVER_REQUES_CANCEL_OBSERVE_HPP