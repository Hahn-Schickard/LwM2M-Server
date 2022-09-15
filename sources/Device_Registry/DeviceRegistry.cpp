#include "DeviceRegistry.hpp"
#include "XmlParser.hpp"

#include "HaSLL/LoggerManager.hpp"

using namespace std;
using namespace HaSLI;

namespace LwM2M {
DeviceNotFound::DeviceNotFound(const string& identifier)
    : runtime_error("Device with id: " + identifier +
          " could not be found in the registry.") {}

DeviceRegistry::DeviceRegistry(const string& configuration_path)
    : Event_Model::EventSource<RegistryEvent>(
          bind(&DeviceRegistry::logListenerException, this, placeholders::_1)),
      logger_(LoggerManager::registerTypedLogger(this)) {
  try {
    supported_descriptors_ = deserializeModel(configuration_path);
  } catch (exception& ex) {
    logger_->log(SeverityLevel::ERROR,
        "Received an exception during descriptor deserialization: {}",
        ex.what());
  }
}

void DeviceRegistry::logListenerException(const exception_ptr& eptr) {
  try {
    if (eptr) {
      rethrow_exception(eptr);
    }
  } catch (const exception& ex) {
    logger_->log(SeverityLevel::ERROR,
        "One of the listeners threw an exception, while handing an "
        "event notification. Exception: {}",
        ex.what());
  } catch (...) {
    logger_->log(
        SeverityLevel::CRITICAL, "Caught an unkown unhandled exception.");
    // @TODO: decide if this should rethrow to OS and crash the socket
  }
}

SupportedObjectDescriptorsMapPtr DeviceRegistry::getSupportedDescriptors() {
  return std::make_shared<SupportedObjectDescriptorsMap>(
      supported_descriptors_);
}

bool DeviceRegistry::isRegistered(const string& identifier) {
  return (device_registry_.find(identifier) != device_registry_.end());
}

void DeviceRegistry::registerDevice(DevicePtr new_device) {
  if (isRegistered(new_device->getDeviceId())) {
    logger_->log(SeverityLevel::TRACE,
        "Device {} with id {} already exists in the registry!",
        new_device->getName(), new_device->getDeviceId());
    deregisterDevice(new_device->getDeviceId());
  }
  device_registry_.emplace(new_device->getDeviceId(), new_device);
  logger_->log(SeverityLevel::TRACE,
      "Device {} with id {} has been registered.", new_device->getName(),
      new_device->getDeviceId());
  auto event = std::make_shared<RegistryEvent>(
      RegistryEventType::REGISTERED, new_device->getDeviceId(), new_device);
  logger_->log(SeverityLevel::TRACE,
      "Dispatching registration event for device {}:{}", new_device->getName(),
      new_device->getDeviceId());
  notify(event);
}

void DeviceRegistry::updateDevice(DevicePtr updated_device) {
  auto it = device_registry_.find(updated_device->getDeviceId());
  if (it != device_registry_.end()) {
    it->second = updated_device;
    logger_->log(SeverityLevel::TRACE, "Device {} with id {} has been updated.",
        updated_device->getName(), updated_device->getDeviceId());
    auto event = std::make_shared<RegistryEvent>(RegistryEventType::UPDATED,
        updated_device->getDeviceId(), updated_device);
    logger_->log(SeverityLevel::TRACE,
        "Dispatching update event for device {}:{}", updated_device->getName(),
        updated_device->getDeviceId());
    notify(event);
  } else {
    throw DeviceNotFound(updated_device->getDeviceId());
  }
}

void DeviceRegistry::deregisterDevice(const string& identifier) {
  logger_->log(SeverityLevel::TRACE,
      "Looking for Device with id {} within the registry.", identifier);
  auto it = device_registry_.find(identifier);
  if (it != device_registry_.end()) {
    device_registry_.erase(it);
    logger_->log(SeverityLevel::TRACE,
        "Device with id {} has been removed from the registry.", identifier);
    auto event = std::make_shared<RegistryEvent>(
        RegistryEventType::DEREGISTERED, identifier);
    logger_->log(SeverityLevel::TRACE,
        "Dispatching deregistration event for device {}", identifier);
    notify(event);
  } else {
    throw DeviceNotFound(identifier);
  }
}

DevicePtr DeviceRegistry::getDevice(const string& identifier) {
  auto it = device_registry_.find(identifier);
  if (it != device_registry_.end()) {
    return it->second;
  } else {
    throw DeviceNotFound(identifier);
  }
}
} // namespace LwM2M