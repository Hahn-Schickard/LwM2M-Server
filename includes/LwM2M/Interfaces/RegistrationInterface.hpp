#ifndef __LWM2M_MODEL_REGISTRATION_INTERFACE_HPP
#define __LWM2M_MODEL_REGISTRATION_INTERFACE_HPP

#include "InterfaceRunner.hpp"
#include "Logger.hpp"
#include "LwM2M_Device.hpp"
#include "LwM2M_ObjectDescriptor.hpp"
#include "RegistrationInterfaceMessages.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace LwM2M {

class RegistrationInterface : public InterfaceRunner {
  std::unordered_map<uint32_t, ObjectDescriptor> supported_descriptors_;
  std::shared_ptr<ThreadsafeQueue<Regirstration_Interface_Message>>
      incoming_message_queue_;
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Device>>>
      device_registery_;
  std::shared_ptr<HaSLL::Logger> logger_;

  bool isRegistered(std::string device_id);

  std::unordered_map<uint32_t, ObjectDescriptor>
  assignObjectInstances(std::unordered_map<unsigned int, unsigned int> objects);

  std::shared_ptr<Message>
  handleRegisterRequest(std::shared_ptr<Register_Request> request);

  std::shared_ptr<Message>
  handleUpdateRequest(std::shared_ptr<Update_Request> request);

  std::shared_ptr<Message>
  handleDeregisterRequest(std::shared_ptr<Deregister_Request> request);

  std::shared_ptr<Message>
  handleRequest(std::shared_ptr<Regirstration_Interface_Message> message);

public:
  RegistrationInterface(
      std::shared_ptr<ThreadsafeQueue<Message>> outgoing_message_queue,
      std::shared_ptr<ThreadsafeQueue<Regirstration_Interface_Message>>
          incoming_message_queue,
      std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Device>>>
          device_registery,
      const std::string &configuration_path);

  void run() override;
};

} // namespace LwM2M

#endif //__LWM2M_MODEL_REGISTRATION_INTERFACE_HPP