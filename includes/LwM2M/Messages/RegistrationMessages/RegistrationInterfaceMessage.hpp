#ifndef __LWM2M_REGISTRATION_INTERFACE_MESSAGE_HPP
#define __LWM2M_REGISTRATION_INTERFACE_MESSAGE_HPP

#include "LwM2M_Message.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace LwM2M {
struct RegirstrationInterfaceMessage : Message {
  RegirstrationInterfaceMessage(std::string endpoint_address,
                                unsigned int endpoint_port, uint16_t message_id,
                                std::vector<uint8_t> token,
                                MessageType message_type);
};
}; // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_MESSAGE_HPP