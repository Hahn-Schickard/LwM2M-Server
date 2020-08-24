#include "DeviceManagmentInterfaceMessages.hpp"

using namespace std;

namespace LwM2M {

Read_Request::Read_Request(string endpoint_address, unsigned int endpoint_port,
                           unsigned int object_id,
                           optional<unsigned int> object_instance_id,
                           optional<unsigned int> resource_id,
                           optional<unsigned int> resoruce_instance_id)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::READ),
      object_id_(object_id), object_instance_id_(move(object_instance_id)),
      resource_id_(move(resource_id)),
      resoruce_instance_id_(move(resoruce_instance_id)) {}

Discover_Request::Discover_Request(string endpoint_address,
                                   unsigned int endpoint_port,
                                   unsigned int object_id,
                                   optional<unsigned int> object_instance_id,
                                   optional<unsigned int> resource_id)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::DISCOVER),
      object_id_(object_id), object_instance_id_(move(object_instance_id)),
      resource_id_(move(resource_id)) {}

Write_Request::Write_Request(string endpoint_address,
                             unsigned int endpoint_port, unsigned int object_id,
                             unsigned int object_instance_id,
                             optional<unsigned int> resource_id,
                             optional<unsigned int> resoruce_instance_id,
                             string value)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::WRITE),
      object_id_(object_id), object_instance_id_(object_instance_id),
      resource_id_(move(resource_id)),
      resoruce_instance_id_(move(resoruce_instance_id)), value_(move(value)) {}

Write_Attributes_Request::Write_Attributes_Request(
    string endpoint_address, unsigned int endpoint_port, unsigned int object_id,
    optional<unsigned int> object_instance_id,
    optional<unsigned int> resource_id,
    optional<unsigned int> resoruce_instance_id,
    optional<Notify_Attribute> notify_attribute)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,

                                        MessageType::WRITE_ATTRIBUTES),
      object_id_(object_id), object_instance_id_(move(object_instance_id)),
      resource_id_(move(resource_id)),
      resoruce_instance_id_(resoruce_instance_id),
      notify_attribute_(move(notify_attribute)) {}

Execute_Request::Execute_Request(
    string endpoint_address, unsigned int endpoint_port, unsigned int object_id,
    unsigned int object_instance_id, unsigned int resource_id,
    unsigned int resoruce_instance_id, optional<string> arguments)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::EXECUTE),
      object_id_(object_id), object_instance_id_(object_instance_id),
      resource_id_(resource_id), resoruce_instance_id_(resoruce_instance_id),
      arguments_(move(arguments)) {}

Create_Request::Create_Request(string endpoint_address,
                               unsigned int endpoint_port,
                               unsigned int object_id, string value)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::CREATE),
      object_id_(object_id), value_(move(value)) {}

Delete_Request::Delete_Request(string endpoint_address,
                               unsigned int endpoint_port,
                               unsigned int object_id,
                               unsigned int object_instance_id)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::DELETE),
      object_id_(object_id), object_instance_id_(object_instance_id) {}

ReadComoposite_Request::ReadComoposite_Request(string endpoint_address,
                                               unsigned int endpoint_port)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::READ_COMPOSITE) {
  throw UnsupportedOperation("Read-Composite operation is not supported!");
}

WriteComoposite_Request::WriteComoposite_Request(string endpoint_address,
                                                 unsigned int endpoint_port)
    : DeviceManagment_Interface_Message(endpoint_address, endpoint_port,
                                        MessageType::WRITE_COMPOSITE) {
  throw UnsupportedOperation("Write-Composite operation is not supported!");
}

} // namespace LwM2M
