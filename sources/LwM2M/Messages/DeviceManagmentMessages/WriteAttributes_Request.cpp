#include "WriteAttributes_Request.hpp"

using namespace std;

namespace LwM2M {
WriteAttributes_Request::WriteAttributes_Request(
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
} // namespace LwM2M