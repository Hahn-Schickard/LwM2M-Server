#include "RegistryEvent.hpp"

using namespace std;
using namespace LwM2M;

string toString(RegistryEventType type) {
  switch (type) {
  case RegistryEventType::REGISTERED: {
    return "Device Registered";
  }
  case RegistryEventType::UPDATED: {
    return "Device Updated";
  }
  case RegistryEventType::DEREGISTERED: {
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
