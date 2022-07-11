#ifndef __LWM2M_MODEL_DEVICE_REGISTRY_HPP
#define __LWM2M_MODEL_DEVICE_REGISTRY_HPP

#include "Device.hpp"
#include "Event_Model/EventSource.hpp"
#include "Logger.hpp"
#include "RegistryEvent.hpp"

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace LwM2M {

struct DeviceNotFound : public std::runtime_error {
  DeviceNotFound(std::string const& identifier);
};

using SupportedObjectDescriptorsMap =
    std::unordered_map<uint32_t, ObjectDescriptorPtr>;
using SupportedObjectDescriptorsMapPtr =
    std::shared_ptr<SupportedObjectDescriptorsMap>;

class DeviceRegistry : public Event_Model::EventSource<RegistryEvent> {
  using DeviceRegistryMap = std::unordered_map<std::string, DevicePtr>;

  SupportedObjectDescriptorsMap supported_descriptors_;
  DeviceRegistryMap device_registry_;
  std::shared_ptr<HaSLL::Logger> logger_;

  void logListenerException(std::exception_ptr ex);

public:
  DeviceRegistry(const std::string& configuration_path);
  ~DeviceRegistry();

  SupportedObjectDescriptorsMapPtr getSupportedDescriptors();

  bool isRegistered(std::string identifier);

  /**
   * @brief Registers a given LwM2M::Device to the current registry. If a given
   * LwM2M::Device already exists in the registry, calls deregisterDevice()
   * first. Informs all registered Event_Model::EventListener<RegistryEvent>
   * instances that a new LwM2M::Device has been added to the registry.
   *
   * @throw std::invalid_argument - if LwM2M::DevicePtr is nullptr
   *
   * @param new_device
   */
  void registerDevice(DevicePtr new_device);

  /**
   * @brief Check if a given LwM2M::Device exists in the current registry and
   * updates it with the given value. Informs all registered
   * Event_Model::EventListener<RegistryEvent> instances that a given
   * LwM2M::Device has been updated.
   *
   * @throw DeviceNotFound - if a given LwM2M::Device does not exist in the
   * current registry.
   * @throw std::invalid_argument - if LwM2M::DevicePtr is nullptr
   *
   * @param updated_device
   */
  void updateDevice(DevicePtr updated_device);

  /**
   * @brief Checks if the registry contains a LwM2M::Device with a given
   * identifier and removes it from the current registry. Informs all registered
   * Event_Model::EventListener<RegistryEvent> instances that a given
   * LwM2M::Device has been removed.
   *
   * @throw DeviceNotFound - if a given LwM2M::Device does not exist in the
   * current registry.
   *
   * @param identifier
   */
  void deregisterDevice(std::string identifier);

  /**
   * @brief Check if the registry contains a LwM2M::Device with a given
   * identifier and returns a std::shared_ptr of it to the caller.
   *
   * @throw DeviceNotFound - if a given LwM2M::Device does not exist in the
   * current registry.
   *
   * @param identifier
   * @return DevicePtr
   */
  DevicePtr getDevice(std::string identifier);
};

using DeviceRegistryPtr = std::shared_ptr<DeviceRegistry>;
using EventSourcePtr = std::shared_ptr<Event_Model::EventSource<RegistryEvent>>;

} // namespace LwM2M

#endif //__LWM2M_MODEL_DEVICE_REGISTRY_HPP