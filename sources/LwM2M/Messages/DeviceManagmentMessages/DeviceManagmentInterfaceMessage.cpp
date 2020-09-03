#include "DeviceManagmentInterfaceMessage.hpp"

using namespace std;

namespace LwM2M {
DeviceManagment_Interface_Message::DeviceManagment_Interface_Message(
    std::string endpoint_address, unsigned int endpoint_port,
    MessageType message_type)
    : Message(endpoint_address, endpoint_port, message_type) {}
} // namespace LwM2M
