#include "ServerRequest_WriteAttributes.hpp"

using namespace std;

namespace LwM2M {
Notify_Attribute::Notify_Attribute(
    optional<unsigned int> minimum_period,
    optional<unsigned int> maximum_period, optional<unsigned int> greater_than,
    optional<unsigned int> less_than, optional<unsigned int> step,
    optional<unsigned int> minimum_evaluation_period,
    optional<unsigned int> maximum_evaluation_period)
    : minimum_period_(move(minimum_period)),
      maximum_period_(move(maximum_period)), greater_than_(move(greater_than)),
      less_than_(move(less_than)), step_(move(step)),
      minimum_evaluation_period_(move(minimum_evaluation_period)),
      maximum_evaluation_period_(move(maximum_evaluation_period)) {}

ServerRequest_WriteAttributes::ServerRequest_WriteAttributes(
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