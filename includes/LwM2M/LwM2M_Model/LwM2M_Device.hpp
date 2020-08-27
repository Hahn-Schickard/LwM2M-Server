#ifndef __LWM2M_DEVICE_HPP
#define __LWM2M_DEVICE_HPP

#include "LwM2M_Endpoint.hpp"
#include "LwM2M_ModelType.hpp"
#include "LwM2M_Object.hpp"
#include "LwM2M_ObjectDescriptor.hpp"
#include "Response_Handler.hpp"

#include <string>
#include <unordered_map>

namespace LwM2M {

using ObjectsMap = std::unordered_map<uint32_t, std::shared_ptr<Object>>;
using ObjectDescriptorPair =
    std::pair<std::shared_ptr<ObjectDescriptor>, std::vector<uint32_t>>;
using ObjectDescriptorsMap = std::unordered_map<uint32_t, ObjectDescriptorPair>;

class Device {
  std::shared_ptr<ResponseHandler> response_handler_;
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
  Device(std::shared_ptr<ResponseHandler> response_handler, std::string name,
         std::string endpoint_address, unsigned int endpoint_port,
         size_t life_time, LwM2M_Version version, BindingType binding,
         bool queue_mode, std::string sms_number,
         ObjectDescriptorsMap object_descriptors_map);

  std::string getDeviceId();
  void updateBinding(BindingType binding);
  void updateLifetime(size_t life_time);
  void updateSMS_Number(std::string sms_number);
  void updateObjectsMap(ObjectDescriptorsMap object_instances);
};
}; // namespace LwM2M

#endif //__LWM2M_DEVICE_HPP