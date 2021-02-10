#ifndef __LWM2M_REGISTRATION_EVENT_HPP
#define __LWM2M_REGISTRATION_EVENT_HPP

#include "Device.hpp"

#include <memory>
#include <string>

namespace LwM2M {
enum class RegistryEventType { REGISTERED, UPDATED, DEREGISTERED };

struct RegistryEvent {
  const RegistryEventType type_;
  const std::string identifier_;
  const DevicePtr device_;

  RegistryEvent(RegistryEventType type, std::string identifier,
                DevicePtr device = DevicePtr())
      : type_(type), identifier_(identifier), device_(device) {}
};

} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_EVENT_HPP