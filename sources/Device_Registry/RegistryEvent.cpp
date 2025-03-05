#include "RegistryEvent.hpp"

using namespace std;
using namespace LwM2M;

string toString(RegistryEventType type) {
  switch (type) {
  case RegistryEventType::Registered: {
    return "Device Registered";
  }
  case RegistryEventType::Updated: {
    return "Device Updated";
  }
  case RegistryEventType::Deregistered: {
    return "Device Deregistered";
  }
  default: {
    return "Unknown Device Registration Event";
  }
  }
}

RegistryEvent::RegistryEvent(RegistryEventType type, string identifier,
    optional<DevicePtr> device) // NOLINT
    : type_(type), identifier_(move(identifier)), device_(device) {} // NOLINT
