#include "ClientRequest_Register.hpp"

using namespace std;

namespace LwM2M {
ClientRequest_Register::ClientRequest_Register(
    string endpoint_address, unsigned int endpoint_port, uint16_t message_id,
    vector<uint8_t> token, string endpoint_name, size_t life_time,
    LwM2M_Version version, BindingType binding, bool queue_mode,
    string sms_number,
    unordered_map<unsigned int, vector<unsigned int>> object_instances_map)
    : RegirstrationInterfaceMessage(endpoint_address, endpoint_port, message_id,
                                    token, MessageType::REGISTER),
      endpoint_name_(move(endpoint_name)), life_time_(life_time),
      version_(version), binding_(binding), queue_mode_(queue_mode),
      sms_number_(move(sms_number)),
      object_instances_map_(move(object_instances_map)) {}
} // namespace LwM2M