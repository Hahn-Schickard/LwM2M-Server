#include "Register_Response.hpp"

using namespace std;

namespace LwM2M {
Register_Response::Register_Response() : Response() {}

Register_Response::Register_Response(string endpoint_address,
                                     unsigned int endpoint_port,
                                     uint16_t message_id, vector<uint8_t> token,
                                     MessageType message_type,
                                     ResponseCode response_code)
    : Response(endpoint_address, endpoint_port, message_id, move(token),
               message_type, response_code) {}

Register_Response::Register_Response(string endpoint_address,
                                     unsigned int endpoint_port,
                                     uint16_t message_id, vector<uint8_t> token,
                                     MessageType message_type,
                                     ResponseCode response_code,
                                     vector<string> location)
    : Response(endpoint_address, endpoint_port, message_id, move(token),
               message_type, response_code),
      location_(move(location)) {}
} // namespace LwM2M