#include "Send_Request.hpp"

using namespace std;

namespace LwM2M {
Send_Request::Send_Request(string endpoint_address, unsigned int endpoint_port,
                           uint16_t message_id, vector<uint8_t> token)
    : InformationReporting_Interface_Message(endpoint_address, endpoint_port,
                                             message_id, token,
                                             MessageType::SEND) {
  throw UnsupportedOperation("Send operation is not supported!");
}

} // namespace LwM2M