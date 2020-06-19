#ifndef __LWM2M_MODEL_REGISTRATION_INTERFACE_HPP
#define __LWM2M_MODEL_REGISTRATION_INTERFACE_HPP

#include "Logger.hpp"
#include "LwM2M_Device.hpp"
#include "LwM2M_Object.hpp"
#include "RegistrationInterfaceMessages.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace LwM2M_Model {

class RegistrationInterface {
  std::unordered_map<uint32_t, LwM2M_Object> supported_descriptors_;
  std::shared_ptr<std::unordered_map<std::string, LwM2M_Device>>
      device_registery_;
  std::shared_ptr<HaSLL::Logger> logger_;

  std::shared_ptr<LwM2M_Message>
  handleRegisterRequest(std::shared_ptr<Register_Request> request);

  std::shared_ptr<LwM2M_Message>
  handleUpdateRequest(std::shared_ptr<Update_Request> request);

  std::shared_ptr<LwM2M_Message>
  handleDeregisterRequest(std::shared_ptr<Deregister_Request> request);

public:
  RegistrationInterface(const std::string &configuration_path);
  std::shared_ptr<LwM2M_Message>
  handleRequest(std::shared_ptr<Regirstration_Interface_Message> message);

  std::shared_ptr<std::unordered_map<std::string, LwM2M_Device>>
  getDeviceRegistery();
};

} // namespace LwM2M_Model

#endif //__LWM2M_MODEL_REGISTRATION_INTERFACE_HPP