#ifndef __LWM2M_REGISTRATION_EVENT_HPP
#define __LWM2M_REGISTRATION_EVENT_HPP

#include "LwM2M_Device.hpp"

#include <memory>
#include <string>

namespace LwM2M {
enum class RegistrationInterfaceEventType { REGISTERED, UPDATED, DEREGISTERED };

struct RegistrationInterfaceEvent {
  RegistrationInterfaceEventType type;
  std::string identifier;
  std::shared_ptr<Device> device;
};

} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_EVENT_HPP