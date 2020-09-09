#include "ServerRequest_Discover.hpp"

using namespace std;

namespace LwM2M {
ServerRequest_Discover::ServerRequest_Discover(
    string endpoint_address, unsigned int endpoint_port, unsigned int object_id,
    optional<unsigned int> object_instance_id,
    optional<unsigned int> resource_id)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::DISCOVER),
      object_id_(object_id), object_instance_id_(move(object_instance_id)),
      resource_id_(move(resource_id)) {}
} // namespace LwM2M