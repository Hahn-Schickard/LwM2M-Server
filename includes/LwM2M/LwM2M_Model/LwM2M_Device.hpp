#ifndef __LWM2M_DEVICE_HPP
#define __LWM2M_DEVICE_HPP

#include "LwM2M_Object.hpp"
#include "RegistrationInterfaceMessages.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace LwM2M_Model {
class LwM2M_Device {
  std::string device_id_;
  std::string name_;
  std::string endpoint_address_;
  unsigned int endpoint_port_;
  size_t life_time_;
  LwM2M_Version version_;
  BindingType binding_;
  bool queue_mode_;
  std::optional<std::string> sms_number_;
  std::unordered_map<uint32_t, LwM2M_Object> object_instances_;

public:
  LwM2M_Device();
  LwM2M_Device(std::string name, std::string endpoint_address,
               unsigned int endpoint_port, size_t life_time,
               LwM2M_Version version, BindingType binding, bool queue_mode,
               std::optional<std::string> sms_number,
               std::unordered_map<uint32_t, LwM2M_Object> object_instances_map);

  std::string getDeviceId();
};
}; // namespace LwM2M_Model

#endif //__LWM2M_DEVICE_HPP