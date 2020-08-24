#include "Read_Response.hpp"

using namespace std;

namespace LwM2M {
Read_Response::Read_Response(string endpoint_address,
                             unsigned int endpoint_port, uint16_t message_id,
                             vector<uint8_t> token, string location,
                             vector<uint8_t> payload)
    : Response(move(endpoint_address), endpoint_port, message_id, move(token),
               MessageType::READ, ResponseCode::CONTENT, move(payload)),
      location_(location) {}
} // namespace LwM2M
