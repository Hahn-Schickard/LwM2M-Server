#ifndef __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_REQUEST_HPP
#define __LWM2M_REGISTRATION_INTERFACE_DEREGISTER_REQUEST_HPP

#include "RegistrationInterfaceMessage.hpp"

namespace LwM2M {
struct Deregister_Request : public Regirstration_Interface_Message {
  std::string location_;

  Deregister_Request(std::string endpoint_address, unsigned int endpoint_port,
                     uint16_t message_id, std::vector<uint8_t> token,
                     std::string location);
};
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_DEREGISTER_REQUEST_HPP