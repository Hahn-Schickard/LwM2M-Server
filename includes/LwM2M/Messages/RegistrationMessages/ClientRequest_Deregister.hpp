#ifndef __LWM2M_REGISTRATION_INTERFACE_CLIENT_REQUEST_DEREGISTER_HPP
#define __LWM2M_REGISTRATION_INTERFACE_CLIENT_REQUEST_DEREGISTER_HPP

#include "RegistrationInterfaceMessage.hpp"

namespace LwM2M {
struct ClientRequest_Deregister : public RegirstrationInterfaceMessage {
  std::string location_;

  ClientRequest_Deregister(std::string endpoint_address,
                           unsigned int endpoint_port, uint16_t message_id,
                           std::vector<uint8_t> token, std::string location);
};
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_CLIENT_REQUEST_DEREGISTER_HPP