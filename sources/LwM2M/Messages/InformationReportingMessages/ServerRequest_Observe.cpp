#include "ServerRequest_Observe.hpp"

using namespace std;

namespace LwM2M {
ServerRequest_Observe::ServerRequest_Observe(
    string endpoint_address, unsigned int endpoint_port, uint16_t message_id,
    vector<uint8_t> token, unsigned int object_id,
    optional<unsigned int> object_instance_id,
    optional<unsigned int> resource_id,
    optional<unsigned int> resoruce_instance_id)
    : InformationReporting_Interface_Message(endpoint_address, endpoint_port,
                                             message_id, token,
                                             MessageType::OBSERVE),
      object_id_(object_id), object_instance_id_(move(object_instance_id)),
      resource_id_(move(resource_id)),
      resoruce_instance_id_(move(resoruce_instance_id)) {}
} // namespace LwM2M