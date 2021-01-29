#ifndef __LWM2M_DEVICE_HPP
#define __LWM2M_DEVICE_HPP

#include "Endpoint.hpp"
#include "ModelType.hpp"
#include "Object.hpp"
#include "ObjectDescriptor.hpp"
#include "RequestsManager.hpp"

#include <string>
#include <unordered_map>

namespace LwM2M {

using ObjectPtr = std::shared_ptr<Object>;
using ObjectsMap = std::unordered_map<uint32_t, ObjectPtr>;
using ObjectDescriptorPair =
    std::pair<std::shared_ptr<ObjectDescriptor>, std::vector<uint32_t>>;
using ObjectDescriptorsMap = std::unordered_map<uint32_t, ObjectDescriptorPair>;

class Device {
  std::shared_ptr<RequestsManager> encoder_;
  std::string device_id_;
  std::string name_;
  size_t life_time_;
  LwM2M_Version version_;
  BindingType binding_;
  bool queue_mode_;
  std::shared_ptr<Endpoint> endpoint_;
  ObjectsMap object_instances_;

private:
  void makeObjects(ObjectDescriptorsMap object_descriptors_map);

public:
  Device();
  Device(std::shared_ptr<RequestsManager> encoder, std::string name,
         std::string endpoint_address, unsigned int endpoint_port,
         size_t life_time, LwM2M_Version version, BindingType binding,
         bool queue_mode, std::string sms_number,
         ObjectDescriptorsMap object_descriptors_map);

  std::string getDeviceId();
  std::string getName();
  ObjectPtr getObject(uint32_t id);
  ObjectsMap getObjects();

  void updateBinding(BindingType binding);
  void updateLifetime(size_t life_time);
  void updateSMS_Number(std::string sms_number);
  void updateObjectsMap(ObjectDescriptorsMap object_instances);
};
}; // namespace LwM2M

#endif //__LWM2M_DEVICE_HPP