#ifndef __LWM2M_REGISTRATION_INTERFACE_UPDATE_REQUEST_HPP
#define __LWM2M_REGISTRATION_INTERFACE_UPDATE_REQUEST_HPP

#include "LwM2M_ModelType.hpp"
#include "RegistrationInterfaceMessage.hpp"

namespace LwM2M {
struct Update_Request : public Regirstration_Interface_Message {
  std::string location_;
  std::optional<size_t> lifetime_;
  std::optional<BindingType> binding_;
  std::optional<std::string> sms_number_;
  std::unordered_map<unsigned int, unsigned int> object_instances_map_;

  Update_Request(
      std::string endpoint_address, unsigned int endpoint_port,
      uint16_t message_id, std::vector<uint8_t> token, std::string location,
      std::optional<size_t> lifetime, std::optional<BindingType> binding,
      std::optional<std::string> sms_number,
      std::unordered_map<unsigned int, unsigned int> object_instances_map);
};
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_UPDATE_REQUEST_HPP