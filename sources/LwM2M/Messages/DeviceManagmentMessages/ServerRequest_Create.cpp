#include "ServerRequest_Create.hpp"

using namespace std;

namespace LwM2M {
ServerRequest_Create::ServerRequest_Create(string endpoint_address,
                                           unsigned int endpoint_port,
                                           unsigned int object_id, string value)
    : DeviceManagmentMessage(endpoint_address, endpoint_port,
                             MessageType::CREATE),
      object_id_(object_id), value_(move(value)) {}
} // namespace LwM2M