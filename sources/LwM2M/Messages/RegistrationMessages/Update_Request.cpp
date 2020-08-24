#include "Update_Request.hpp"

using namespace std;

namespace LwM2M {
Update_Request::Update_Request(
    string endpoint_address, unsigned int endpoint_port, uint16_t message_id,
    vector<uint8_t> token, string location, optional<size_t> lifetime,
    optional<BindingType> binding, optional<string> sms_number,
    unordered_map<unsigned int, unsigned int> object_instances_map)
    : Regirstration_Interface_Message(endpoint_address, endpoint_port,
                                      message_id, token, MessageType::UPDATE),
      location_(move(location)), lifetime_(move(lifetime)),
      binding_(move(binding)), sms_number_(move(sms_number)),
      object_instances_map_(move(object_instances_map)) {}
} // namespace LwM2M