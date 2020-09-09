#include "ClientRequest_Deregister.hpp"

using namespace std;

namespace LwM2M {
ClientRequest_Deregister::ClientRequest_Deregister(string endpoint_address,
                                                   unsigned int endpoint_port,
                                                   uint16_t message_id,
                                                   vector<uint8_t> token,
                                                   string location)
    : Regirstration_Interface_Message(endpoint_address, endpoint_port,
                                      message_id, token,
                                      MessageType::DEREGISTER),
      location_(move(location)) {}

} // namespace LwM2M