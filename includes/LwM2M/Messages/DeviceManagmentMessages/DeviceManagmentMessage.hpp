#ifndef __LWM2M_DEVICE_MANAGMENT_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_MESSAGE_HPP

#include "LwM2M_Message.hpp"

#include <string>

namespace LwM2M {
struct DeviceManagmentMessage : public Message {
  DeviceManagmentMessage(std::string endpoint_address,
                         unsigned int endpoint_port, MessageType message_type);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_MESSAGE_HPP