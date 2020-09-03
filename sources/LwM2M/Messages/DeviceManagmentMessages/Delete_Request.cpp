#include "Delete_Request.hpp"

using namespace std;

namespace LwM2M {
Delete_Request::Delete_Request(string endpoint_address,
                               unsigned int endpoint_port,
                               unsigned int object_id,
                               unsigned int object_instance_id)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::DELETE),
      object_id_(object_id), object_instance_id_(object_instance_id) {}
} // namespace LwM2M