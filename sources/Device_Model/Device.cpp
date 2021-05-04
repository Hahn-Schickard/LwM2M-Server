#include "Device.hpp"

using namespace std;

namespace LwM2M {
string generateDeviceID(string name, EndpointPtr endpoint) {
  // Possible hash colissions, need to check the size of name and endpoint
  // address, offset each by the lenghts and concat it with bit shifts
  size_t result = hash<string>{}(name) +
                  hash<string>{}(endpoint->endpoint_address_) +
                  endpoint->endpoint_port_;
  return to_string(result);
}

Device::Device(RequesterPtr requester, EndpointPtr endpoint,
               ObjectDescriptorsMap object_descriptors_map, size_t life_time,
               string name, LwM2M_Version version, BindingType binding,
               bool queue_mode)
    : requester_(requester), endpoint_(endpoint), life_time_(life_time),
      name_(name), version_(version), binding_(binding),
      queue_mode_(queue_mode) {
  makeObjects(object_descriptors_map);
  if (!endpoint_) {
    throw invalid_argument("Endpoint can not be a nullptr!");
  } else {
    device_id_ = generateDeviceID(name, endpoint_);
  }
}

void Device::makeObjects(ObjectDescriptorsMap object_descriptors_map) {
  for (auto object_descriptor_pair : object_descriptors_map) {
    auto object_instance_pair = object_descriptor_pair.second;
    auto object =
        make_shared<Object>(requester_, endpoint_, object_instance_pair.second,
                            object_instance_pair.first);
    object_instances_.emplace(object_descriptor_pair.first, move(object));
  }
}

string Device::getDeviceId() { return device_id_; }

string Device::getName() { return name_; }

ObjectPtr Device::getObject(uint32_t id) {
  auto it = object_instances_.find(id);
  if (it != object_instances_.end()) {
    return it->second;
  } else {
    throw ObjectDoesNotExist(ObjectID(id));
  }
}

ObjectsMap Device::getObjects() { return object_instances_; }

void Device::updateBinding(BindingType binding) { binding_ = binding; }

void Device::updateLifetime(size_t life_time) { life_time_ = life_time; }

void Device::updateObjectsMap(ObjectDescriptorsMap object_descriptors_map) {
  makeObjects(object_descriptors_map);
}
} // namespace LwM2M