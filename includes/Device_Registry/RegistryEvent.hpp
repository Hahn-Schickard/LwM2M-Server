#ifndef __LWM2M_REGISTRATION_EVENT_HPP
#define __LWM2M_REGISTRATION_EVENT_HPP

#include "Device.hpp"

#include <memory>
#include <optional>
#include <string>

namespace LwM2M {
enum class RegistryEventType { REGISTERED, UPDATED, DEREGISTERED };

struct RegistryEvent {
  const RegistryEventType type_;
  const std::string identifier_;
  const std::optional<DevicePtr> device_;

  RegistryEvent(RegistryEventType type, std::string identifier,
      std::optional<DevicePtr> device = std::nullopt)
      : type_(type), identifier_(identifier), device_(device) {}
};

using RegistryEventPtr = std::shared_ptr<RegistryEvent>;

} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_EVENT_HPP