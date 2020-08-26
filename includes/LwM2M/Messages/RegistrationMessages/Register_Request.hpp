#ifndef __LWM2M_REGISTRATION_INTERFACE_REGISTER_REQUEST_HPP
#define __LWM2M_REGISTRATION_INTERFACE_REGISTER_REQUEST_HPP

#include "LwM2M_ModelType.hpp"
#include "RegistrationInterfaceMessage.hpp"

#include <unordered_map>

namespace LwM2M {
struct Register_Request : public Regirstration_Interface_Message {
  std::string endpoint_name_;
  size_t life_time_;
  LwM2M_Version version_;
  BindingType binding_;
  bool queue_mode_;
  std::string sms_number_;
  std::unordered_map<unsigned int, unsigned int> object_instances_map_;

  Register_Request(
      std::string endpoint_address, unsigned int endpoint_port,
      uint16_t message_id, std::vector<uint8_t> token,
      std::string endpoint_name, size_t life_time, LwM2M_Version version,
      BindingType binding, bool queue_mode, std::string sms_number,
      std::unordered_map<unsigned int, unsigned int> object_instances_map);
};
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_REGISTER_REQUEST_HPP