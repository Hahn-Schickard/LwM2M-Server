#ifndef __LWM2M_REGISTRATION_INTERFACE_REGISTER_RESPONSE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_REGISTER_RESPONSE_HPP

#include "RegistrationInterfaceMessage.hpp"

namespace LwM2M {
struct Register_Response : Response {
  std::vector<std::string> location_;

  Register_Response();
  Register_Response(std::string endpoint_address, unsigned int endpoint_port,
                    uint16_t message_id, std::vector<uint8_t> token,
                    MessageType message_type, ResponseCode response_code);
  Register_Response(std::string endpoint_address, unsigned int endpoint_port,
                    uint16_t message_id, std::vector<uint8_t> token,
                    MessageType message_type, ResponseCode response_code,
                    std::vector<std::string> location);
};
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_REGISTER_RESPONSE_HPP