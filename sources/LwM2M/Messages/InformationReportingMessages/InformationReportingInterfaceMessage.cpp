#include "InformationReportingInterfaceMessage.hpp"

using namespace std;

namespace LwM2M {
InformationReporting_Interface_Message::InformationReporting_Interface_Message(
    std::string endpoint_address, unsigned int endpoint_port,
    uint16_t message_id, std::vector<uint8_t> token, MessageType message_type)
    : Message(endpoint_address, endpoint_port, message_id, token,
              message_type) {}
} // namespace LwM2M
