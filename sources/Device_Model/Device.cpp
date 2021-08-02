#include "Device.hpp"

#include <iomanip>
#include <sstream>

using namespace std;

namespace LwM2M {
string generateDeviceID(string name, EndpointPtr endpoint) {
  auto address = endpoint->toString();
  auto offset = address.size();
  size_t result = hash<string>{}(name) << offset;
  result |= hash<string>{}(address);
  stringstream stream;
  stream << hex << result;
  return stream.str();
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
    auto descriptor = object_descriptor_pair.second;
    auto object_id = object_descriptor_pair.first;

    RequiredObjectInstances object_instances;
    auto range = object_descriptors_map.equal_range(object_id);
    for (auto instance = range.first; instance != range.second; ++instance) {
      object_instances.emplace(instance->first);
    }

    auto object = make_shared<Object>(requester_, endpoint_, object_instances,
                                      descriptor);

    object_instances_.emplace(object_descriptor_pair.first.getObjectID(),
                              move(object));
  }
}

string Device::getDeviceId() { return device_id_; }

string Device::getName() { return name_; }

ObjectPtr Device::getObject(ElementID id) {
  auto it = object_instances_.find(id.getObjectID());
  if (it != object_instances_.end()) {
    return it->second;
  } else {
    throw ObjectDoesNotExist(id);
  }
}

ObjectsMap Device::getObjects() { return object_instances_; }

void Device::updateBinding(BindingType binding) { binding_ = binding; }

void Device::updateLifetime(size_t life_time) { life_time_ = life_time; }

void Device::updateObjectsMap(ObjectDescriptorsMap object_descriptors_map) {
  makeObjects(object_descriptors_map);
}
} // namespace LwM2M