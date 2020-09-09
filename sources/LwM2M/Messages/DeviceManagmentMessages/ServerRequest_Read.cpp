#include "ServerRequest_Read.hpp"

using namespace std;

namespace LwM2M {

ServerRequest_Read::ServerRequest_Read(string endpoint_address,
                                       unsigned int endpoint_port,
                                       unsigned int object_id,
                                       unsigned int object_instance_id)
    : DeviceManagmentMessage(endpoint_address, endpoint_port,
                             MessageType::READ),
      object_id_(object_id), object_instance_id_(object_instance_id) {}

ServerRequest_Read::ServerRequest_Read(string endpoint_address,
                                       unsigned int endpoint_port,
                                       unsigned int object_id,
                                       unsigned int object_instance_id,
                                       unsigned int resource_id)
    : DeviceManagmentMessage(endpoint_address, endpoint_port,
                             MessageType::READ),
      object_id_(object_id), object_instance_id_(object_instance_id),
      resource_id_(resource_id) {}

ServerRequest_Read::ServerRequest_Read(string endpoint_address,
                                       unsigned int endpoint_port,
                                       unsigned int object_id,
                                       unsigned int object_instance_id,
                                       unsigned int resource_id,
                                       unsigned int resoruce_instance_id)
    : DeviceManagmentMessage(endpoint_address, endpoint_port,
                             MessageType::READ),
      object_id_(object_id), object_instance_id_(object_instance_id),
      resource_id_(resource_id), resoruce_instance_id_(resoruce_instance_id) {}

} // namespace LwM2M