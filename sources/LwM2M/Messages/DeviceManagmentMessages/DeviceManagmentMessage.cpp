#include "DeviceManagmentMessage.hpp"

using namespace std;

namespace LwM2M {
DeviceManagmentMessage::DeviceManagmentMessage(std::string endpoint_address,
                                               unsigned int endpoint_port,
                                               MessageType message_type)
    : Message(endpoint_address, endpoint_port, message_type) {}
} // namespace LwM2M
