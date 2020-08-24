#include "Create_Request.hpp"

using namespace std;

namespace LwM2M {
Create_Request::Create_Request(string endpoint_address,
                               unsigned int endpoint_port,
                               unsigned int object_id, string value)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::CREATE),
      object_id_(object_id), value_(move(value)) {}
} // namespace LwM2M