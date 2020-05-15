#include "RegistrationInterfaceMessages.hpp"

using namespace std;

namespace LwM2M_Model {

Register_Request::Register_Request(
    string endpoint_name, size_t life_time, LwM2M_Version version,
    BindingType binding, bool queue_mode, optional<size_t> sms_number,
    unordered_map<unsigned int, unsigned int> object_instances_map)
    : Regirstration_Interface_Message(MessageType::REGISTER),
      endpoint_name_(move(endpoint_name)), life_time_(life_time),
      version_(version), binding_(binding), queue_mode_(queue_mode),
      sms_number_(move(sms_number)),
      object_instances_map_(move(object_instances_map)) {}

Update_Request::Update_Request(
    string location, optional<size_t> lifetime, optional<BindingType> binding,
    optional<size_t> sms_number,
    optional<unordered_map<unsigned int, unsigned int>> object_instances_map)
    : Regirstration_Interface_Message(MessageType::UPDATE),
      location_(move(location)), lifetime_(move(lifetime)),
      binding_(move(binding)), sms_number_(move(sms_number)),
      object_instances_map_(move(object_instances_map)) {}

Deregister_Request::Deregister_Request(string location)
    : Regirstration_Interface_Message(MessageType::DEREGISTER),
      location_(move(location)) {}

}; // namespace LwM2M_Model
