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

Device::Device(shared_ptr<ResponseHandler> response_handler, string name,
               string endpoint_address, unsigned int endpoint_port,
               size_t life_time, LwM2M_Version version, BindingType binding,
               bool queue_mode, string sms_number,
               ObjectDescriptorsMap object_descriptors_map)
    : response_handler_(response_handler),
      device_id_(generateDeviceID(name, endpoint_address, endpoint_port)),
      name_(name), life_time_(life_time), version_(version), binding_(binding),
      queue_mode_(queue_mode),
      endpoint_(make_shared<Endpoint>(
          Endpoint{endpoint_address, endpoint_port, sms_number})) {
  makeObjects(object_descriptors_map);
}

void Device::makeObjects(ObjectDescriptorsMap object_descriptors_map) {
  for (auto object_descriptor_pair : object_descriptors_map) {
    auto object_instance_pair = object_descriptor_pair.second;
    auto object =
        make_shared<Object>(endpoint_, object_instance_pair.second,
                            response_handler_, object_instance_pair.first);
    object_instances_.emplace(object_descriptor_pair.first, move(object));
  }
}

string Device::getDeviceId() { return device_id_; }

void Device::updateBinding(BindingType binding) { binding_ = binding; }

void Device::updateLifetime(size_t life_time) { life_time_ = life_time; }

void Device::updateSMS_Number(std::string sms_number) {
  endpoint_->sms_number_ = sms_number;
}

void Device::updateObjectsMap(ObjectDescriptorsMap object_descriptors_map) {
  makeObjects(object_descriptors_map);
}
} // namespace LwM2M