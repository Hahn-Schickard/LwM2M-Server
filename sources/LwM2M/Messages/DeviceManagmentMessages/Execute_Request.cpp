#include "Execute_Request.hpp"

using namespace std;

namespace LwM2M {
Execute_Request::Execute_Request(
    string endpoint_address, unsigned int endpoint_port, unsigned int object_id,
    unsigned int object_instance_id, unsigned int resource_id,
    unsigned int resoruce_instance_id, optional<string> arguments)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::EXECUTE),
      object_id_(object_id), object_instance_id_(object_instance_id),
      resource_id_(resource_id), resoruce_instance_id_(resoruce_instance_id),
      arguments_(move(arguments)) {}
} // namespace LwM2M