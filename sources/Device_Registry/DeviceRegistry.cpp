#include "DeviceRegistry.hpp"
#include "LoggerRepository.hpp"
#include "XmlParser.hpp"

using namespace std;
using namespace HaSLL;

namespace LwM2M {
ObjectDescriptorNotSupported::ObjectDescriptorNotSupported(uint32_t object_id)
    : runtime_error("Object with id: " + to_string(object_id) +
                    " is not supported by the current registry.") {}

DeviceNotFound::DeviceNotFound(string const &identifier)
    : runtime_error("Device with id: " + identifier +
                    " could not be found in the registry.") {}

DeviceRegistry::DeviceRegistry(const string &configuration_path)
    : Event_Model::EventSource<RegistryEvent>(),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  try {
    supported_descriptors_ = deserializeModel(configuration_path);
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR,
                 "Received an exception during descriptor deserialization: {}",
                 ex.what());
  }
}

SupportedObjectDescripotrsMapPtr DeviceRegistry::getSupportedDescriptors() {
  return make_shared<SupportedObjectDescripotrsMap>(supported_descriptors_);
}

bool DeviceRegistry::isRegistered(std::string identifier) {
  return (device_registery_.find(identifier) != device_registery_.end())
             ? true
             : false;
}

void DeviceRegistry::registerDevice(DevicePtr new_device) {
  if (isRegistered(new_device->getDeviceId())) {
    logger_->log(SeverityLevel::TRACE,
                 "Device {} with id {} already exists in the registry!",
                 new_device->getName(), new_device->getDeviceId());
    deregisterDevice(new_device->getDeviceId());
  }
  device_registery_.emplace(new_device->getDeviceId(), new_device);
  logger_->log(SeverityLevel::TRACE,
               "Device {} with id {} has been registered.",
               new_device->getName(), new_device->getDeviceId());
  notify(make_shared<RegistryEvent>(RegistryEvent{
      RegistryEventType::REGISTERED, new_device->getDeviceId(), new_device}));
}

void DeviceRegistry::updateDevice(DevicePtr updated_device) {
  auto it = device_registery_.find(updated_device->getDeviceId());
  if (it != device_registery_.end()) {
    it->second = updated_device;
    logger_->log(SeverityLevel::TRACE, "Device {} with id {} has been updated.",
                 updated_device->getName(), updated_device->getDeviceId());
    notify(make_shared<RegistryEvent>(
        RegistryEvent{RegistryEventType::UPDATED, updated_device->getDeviceId(),
                      updated_device}));
  } else {
    throw DeviceNotFound(updated_device->getDeviceId());
  }
}

void DeviceRegistry::deregisterDevice(std::string identifier) {
  auto it = device_registery_.find(identifier);
  if (it != device_registery_.end()) {
    device_registery_.erase(it);
    logger_->log(SeverityLevel::TRACE,
                 "Device with id {} has been removed from the registry.",
                 identifier);
    notify(make_shared<RegistryEvent>(RegistryEvent{
        RegistryEventType::DEREGISTERED, identifier, DevicePtr()}));
  } else {
    throw DeviceNotFound(identifier);
  }
}

DevicePtr DeviceRegistry::getDevice(std::string identifier) {
  auto it = device_registery_.find(identifier);
  if (it != device_registery_.end()) {
    return it->second;
  } else {
    throw DeviceNotFound(identifier);
  }
}
} // namespace LwM2M