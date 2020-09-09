#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_SERVER_REQUEST_OBSERVE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_SERVER_REQUEST_OBSERVE_HPP

#include "InformationReportingInterfaceMessage.hpp"

namespace LwM2M {
struct ServerRequest_Observe : public InformationReporting_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;

  ServerRequest_Observe(std::string endpoint_address,
                        unsigned int endpoint_port, uint16_t message_id,
                        std::vector<uint8_t> token, unsigned int object_id,
                        std::optional<unsigned int> object_instance_id,
                        std::optional<unsigned int> resource_id,
                        std::optional<unsigned int> resoruce_instance_id);
};
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_SERVER_REQUEST_OBSERVE_HPP