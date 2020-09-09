#include "ClientRequest_Send.hpp"

using namespace std;

namespace LwM2M {
ClientRequest_Send::ClientRequest_Send(string endpoint_address,
                                       unsigned int endpoint_port,
                                       uint16_t message_id,
                                       vector<uint8_t> token)
    : InformationReportingMessage(endpoint_address, endpoint_port, message_id,
                                  token, MessageType::SEND) {
  throw UnsupportedOperation("Send operation is not supported!");
}

} // namespace LwM2M