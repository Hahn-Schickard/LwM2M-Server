#ifndef __Device_HPP
#define __Device_HPP

#include "LwM2M_ObjectDescriptor.hpp"
#include "RegistrationInterfaceMessages.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace LwM2M {
class Device {
  std::string device_id_;
  std::string name_;
  std::string endpoint_address_;
  unsigned int endpoint_port_;
  size_t life_time_;
  LwM2M_Version version_;
  BindingType binding_;
  bool queue_mode_;
  std::optional<std::string> sms_number_;
  std::unordered_map<uint32_t, std::shared_ptr<ObjectDescriptor>>
      object_instances_;

public:
  Device();
  Device(std::string name, std::string endpoint_address,
         unsigned int endpoint_port, size_t life_time, LwM2M_Version version,
         BindingType binding, bool queue_mode,
         std::optional<std::string> sms_number,
         std::unordered_map<uint32_t, std::shared_ptr<ObjectDescriptor>>
             object_instances_map);

  std::string getDeviceId();
  void updateBinding(BindingType binding);
  void updateLifetime(size_t life_time);
  void updateSMS_Number(std::string sms_number);
  void updateObjectsMap(
      std::unordered_map<uint32_t, std::shared_ptr<ObjectDescriptor>>
          object_instances);
};
}; // namespace LwM2M

#endif //__Device_HPP