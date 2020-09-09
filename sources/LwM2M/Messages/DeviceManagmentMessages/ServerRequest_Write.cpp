#include "ServerRequest_Write.hpp"

using namespace std;

namespace LwM2M {
ServerRequest_Write::ServerRequest_Write(
    string endpoint_address, unsigned int endpoint_port, unsigned int object_id,
    unsigned int object_instance_id, optional<unsigned int> resource_id,
    optional<unsigned int> resoruce_instance_id, string value)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::WRITE),
      object_id_(object_id), object_instance_id_(object_instance_id),
      resource_id_(move(resource_id)),
      resoruce_instance_id_(move(resoruce_instance_id)), value_(move(value)) {}
} // namespace LwM2M