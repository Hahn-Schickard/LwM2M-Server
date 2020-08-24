#include "Notify_Request.hpp"

using namespace std;

namespace LwM2M {
Notify_Request::Notify_Request(string endpoint_address,
                               unsigned int endpoint_port, uint16_t message_id,
                               vector<uint8_t> token, string value)
    : InformationReporting_Interface_Message(endpoint_address, endpoint_port,
                                             message_id, token,
                                             MessageType::NOTIFY),
      value_(move(value)) {}
} // namespace LwM2M