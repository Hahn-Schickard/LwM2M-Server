#ifndef __LWM2M_MODEL_REGISTRATION_INTERFACE_HPP
#define __LWM2M_MODEL_REGISTRATION_INTERFACE_HPP

#include "Logger.hpp"
#include "LwM2M_Device.hpp"
#include "LwM2M_ObjectDescriptor.hpp"
#include "Message_Encoder.hpp"
#include "Observer_Pattern.hpp"
#include "RegistrationInterfaceEvent.hpp"
#include "RegistrationMessages.hpp"
#include "Response_Handler.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace LwM2M {

using RegistrationEventSourcePtr =
    std::shared_ptr<ObserverPattern::EventSource<RegistrationInterfaceEvent>>;
using ObjectDescriptorPair =
    std::pair<std::shared_ptr<ObjectDescriptor>, std::vector<uint32_t>>;

class RegistrationInterface {
  std::unordered_map<uint32_t, std::shared_ptr<ObjectDescriptor>>
      supported_descriptors_;
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Device>>>
      device_registery_;
  std::shared_ptr<MessageEncoder> encoder_;
  RegistrationEventSourcePtr event_source_;
  std::shared_ptr<HaSLL::Logger> logger_;

  bool isRegistered(std::string device_id);

  std::unordered_map<uint32_t, ObjectDescriptorPair> assignObjectInstances(
      std::unordered_map<unsigned int, std::vector<unsigned int>> objects);

public:
  RegistrationInterface(
      std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Device>>>
          device_registery,
      std::shared_ptr<MessageEncoder> encoder,
      const std::string &configuration_path);

  bool handleRequest(std::unique_ptr<Register_Request> request);

  bool handleRequest(std::unique_ptr<Update_Request> request);

  bool handleRequest(std::unique_ptr<Deregister_Request> request);

  RegistrationEventSourcePtr getEventSource();
};

} // namespace LwM2M

#endif //__LWM2M_MODEL_REGISTRATION_INTERFACE_HPP