#include "RegistrationInterfaceMessages.hpp"

using namespace std;

namespace LwM2M {

Register_Request::Register_Request(
    string endpoint_address, unsigned int endpoint_port, uint16_t message_id,
    vector<uint8_t> token, string endpoint_name, size_t life_time,
    LwM2M_Version version, BindingType binding, bool queue_mode,
    optional<string> sms_number,
    unordered_map<unsigned int, unsigned int> object_instances_map)
    : Regirstration_Interface_Message(endpoint_address, endpoint_port,
                                      message_id, token, MessageType::REGISTER),
      endpoint_name_(move(endpoint_name)), life_time_(life_time),
      version_(version), binding_(binding), queue_mode_(queue_mode),
      sms_number_(move(sms_number)),
      object_instances_map_(move(object_instances_map)) {}

Update_Request::Update_Request(
    string endpoint_address, unsigned int endpoint_port, uint16_t message_id,
    vector<uint8_t> token, string location, optional<size_t> lifetime,
    optional<BindingType> binding, optional<string> sms_number,
    optional<unordered_map<unsigned int, unsigned int>> object_instances_map)
    : Regirstration_Interface_Message(endpoint_address, endpoint_port,
                                      message_id, token, MessageType::UPDATE),
      location_(move(location)), lifetime_(move(lifetime)),
      binding_(move(binding)), sms_number_(move(sms_number)),
      object_instances_map_(move(object_instances_map)) {}

Deregister_Request::Deregister_Request(string endpoint_address,
                                       unsigned int endpoint_port,
                                       uint16_t message_id,
                                       vector<uint8_t> token, string location)
    : Regirstration_Interface_Message(endpoint_address, endpoint_port,
                                      message_id, token,
                                      MessageType::DEREGISTER),
      location_(move(location)) {}

}; // namespace LwM2M
