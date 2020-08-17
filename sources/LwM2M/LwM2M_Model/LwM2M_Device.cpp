#include "LwM2M_Device.hpp"

using namespace std;

namespace LwM2M {
string generateDeviceID(string name, string endpoint_address,
                        unsigned int endpoint_port) {
  size_t result =
      hash<string>{}(name) + hash<string>{}(endpoint_address) + endpoint_port;
  return to_string(result);
}

Device::Device() {}

Device::Device(
    string name, string endpoint_address, unsigned int endpoint_port,
    size_t life_time, LwM2M_Version version, BindingType binding,
    bool queue_mode, optional<string> sms_number,
    unordered_map<uint32_t, shared_ptr<ObjectDescriptor>> object_instances_map)
    : device_id_(generateDeviceID(name, endpoint_address, endpoint_port)),
      name_(name), endpoint_address_(endpoint_address),
      endpoint_port_(endpoint_port), life_time_(life_time), version_(version),
      binding_(binding), queue_mode_(queue_mode), sms_number_(sms_number),
      object_instances_(object_instances_map) {}

string Device::getDeviceId() { return device_id_; }

void Device::updateBinding(BindingType binding) { binding_ = binding; }

void Device::updateLifetime(size_t life_time) { life_time_ = life_time; }

void Device::updateSMS_Number(std::string sms_number) {
  sms_number_ = sms_number;
}

void Device::updateObjectsMap(
    std::unordered_map<uint32_t, std::shared_ptr<ObjectDescriptor>>
        object_instances) {
  object_instances_ = object_instances;
}
} // namespace LwM2M