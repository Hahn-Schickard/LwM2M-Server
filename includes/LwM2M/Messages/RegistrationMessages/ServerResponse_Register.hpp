#ifndef __LWM2M_REGISTRATION_INTERFACE_SERVER_RESPONSE_REGISTER_HPP
#define __LWM2M_REGISTRATION_INTERFACE_SERVER_RESPONSE_REGISTER_HPP

#include "RegistrationInterfaceMessage.hpp"

namespace LwM2M {
struct ServerResponse_Register : Response {
  std::vector<std::string> location_;

  ServerResponse_Register();
  ServerResponse_Register(std::string endpoint_address,
                          unsigned int endpoint_port, uint16_t message_id,
                          std::vector<uint8_t> token, MessageType message_type,
                          ResponseCode response_code);
  ServerResponse_Register(std::string endpoint_address,
                          unsigned int endpoint_port, uint16_t message_id,
                          std::vector<uint8_t> token, MessageType message_type,
                          ResponseCode response_code,
                          std::vector<std::string> location);
};
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_SERVER_RESPONSE_REGISTER_HPP