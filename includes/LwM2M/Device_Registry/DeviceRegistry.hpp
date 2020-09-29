#ifndef __LWM2M_MODEL_REGISTRATION_INTERFACE_HPP
#define __LWM2M_MODEL_REGISTRATION_INTERFACE_HPP

#include "Event_Model.hpp"
#include "Logger.hpp"
#include "LwM2M_Device.hpp"
#include "LwM2M_ObjectDescriptor.hpp"
#include "Message_Encoder.hpp"
#include "RegistrationMessages.hpp"
#include "RegistryEvent.hpp"
#include "Response_Handler.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace LwM2M {

using ObjectDescriptorPair =
    std::pair<std::shared_ptr<ObjectDescriptor>, std::vector<uint32_t>>;

using DevicePtr = std::shared_ptr<Device>;

class DeviceRegistry : public Event_Model::EventSource<RegistryEvent> {
  std::unordered_map<uint32_t, std::shared_ptr<ObjectDescriptor>>
      supported_descriptors_;
  std::unordered_map<std::string, std::shared_ptr<Device>> device_registery_;
  std::shared_ptr<MessageEncoder> encoder_;
  std::shared_ptr<HaSLL::Logger> logger_;

  bool isRegistered(std::string device_id);

  std::unordered_map<uint32_t, ObjectDescriptorPair> assignObjectInstances(
      std::unordered_map<unsigned int, std::vector<unsigned int>> objects);

public:
  DeviceRegistry(std::shared_ptr<MessageEncoder> encoder,
                 const std::string &configuration_path);

  DevicePtr getDevice(std::string identifier);

  bool handleRequest(std::unique_ptr<ClientRequest_Register> request);

  bool handleRequest(std::unique_ptr<ClientRequest_Update> request);

  bool handleRequest(std::unique_ptr<ClientRequest_Deregister> request);
};

} // namespace LwM2M

#endif //__LWM2M_MODEL_REGISTRATION_INTERFACE_HPP