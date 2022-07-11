#ifndef __LWM2M_DEVICE_HPP
#define __LWM2M_DEVICE_HPP

#include "Endpoint.hpp"
#include "ModelType.hpp"
#include "Object.hpp"
#include "ObjectDescriptor.hpp"
#include "Requester.hpp"

#include <string>
#include <unordered_map>

struct ObjectIDHasher {
  std::size_t operator()(const LwM2M::ElementID& id) const {
    using std::hash;
    using std::size_t;

    return hash<uint16_t>{}(id.getObjectID());
  }
};

struct ObjectIDComparator {
  bool operator()(
      const LwM2M::ElementID& lhs, const LwM2M::ElementID& rhs) const {
    return (lhs.getObjectID() == rhs.getObjectID());
  }
};

namespace LwM2M {

using ObjectsMap = std::unordered_map<uint16_t, ObjectPtr>;
using ObjectDescriptorsMap = std::unordered_multimap<ElementID,
    ObjectDescriptorPtr, ObjectIDHasher, ObjectIDComparator>;

struct ObjectDoesNotExist : public std::runtime_error {
  ObjectDoesNotExist(ElementID id)
      : runtime_error("Object " + id.toString() + " does not exits") {}
};

class Device {
  Observable::ExceptionHandler exception_handler_;
  RequesterPtr requester_;
  EndpointPtr endpoint_;
  ObjectsMap object_instances_;
  std::string device_id_;
  size_t life_time_;
  std::string name_;
  LwM2M_Version version_;
  BindingType binding_;
  bool queue_mode_;

  void makeObjects(ObjectDescriptorsMap object_descriptors_map);

public:
  Device() = default;
  Device(Observable::ExceptionHandler handler, RequesterPtr requester,
      EndpointPtr endpoint, ObjectDescriptorsMap object_descriptors_map,
      std::string device_id, size_t life_time, std::string name = std::string(),
      LwM2M_Version version = LwM2M_Version::V1_0,
      BindingType binding = BindingType::UDP, bool queue_mode = false);

  std::string getDeviceId();
  std::string getName();
  ObjectPtr getObject(ElementID id);
  ObjectsMap getObjects();

  void updateBinding(BindingType binding);
  void updateLifetime(size_t life_time);
  void updateObjectsMap(ObjectDescriptorsMap object_instances);
};

using DevicePtr = std::shared_ptr<Device>;
}; // namespace LwM2M

#endif //__LWM2M_DEVICE_HPP