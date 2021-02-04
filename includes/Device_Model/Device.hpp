#ifndef __LWM2M_DEVICE_HPP
#define __LWM2M_DEVICE_HPP

#include "Endpoint.hpp"
#include "ModelType.hpp"
#include "Object.hpp"
#include "ObjectDescriptor.hpp"

#include <string>
#include <unordered_map>

namespace LwM2M {

using ObjectPtr = std::shared_ptr<Object>;
using ObjectsMap = std::unordered_map<uint32_t, ObjectPtr>;
using ObjectDescriptorPair =
    std::pair<std::shared_ptr<ObjectDescriptor>, std::vector<uint32_t>>;
using ObjectDescriptorsMap = std::unordered_map<uint32_t, ObjectDescriptorPair>;

class Device {
  const EndpointPtr endpoint_;
  ObjectsMap object_instances_;
  size_t life_time_;
  std::string device_id_;
  std::string name_;
  LwM2M_Version version_;
  BindingType binding_;
  bool queue_mode_;

  void makeObjects(ObjectDescriptorsMap object_descriptors_map);

public:
  Device() = default;
  Device(EndpointPtr endpoint, ObjectDescriptorsMap object_descriptors_map,
         size_t life_time, std::string name = std::string(),
         LwM2M_Version version = LwM2M_Version::V1_0,
         BindingType binding = BindingType::UDP, bool queue_mode = false);

  std::string getDeviceId();
  std::string getName();
  ObjectPtr getObject(uint32_t id);
  ObjectsMap getObjects();

  void updateBinding(BindingType binding);
  void updateLifetime(size_t life_time);
  void updateObjectsMap(ObjectDescriptorsMap object_instances);
};
}; // namespace LwM2M

#endif //__LWM2M_DEVICE_HPP