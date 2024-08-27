#ifndef __LWM2M_EXAMPLE_APPLICATION_HPP
#define __LWM2M_EXAMPLE_APPLICATION_HPP

#include "DeviceRegistry.hpp"
#include "Event_Model/EventListenerInterface.hpp"
#include "Observer.hpp"

#include <map>

namespace LwM2M_Example {
using namespace LwM2M;

struct RegistrationListener
    : public Event_Model::EventListenerInterface<RegistryEvent> {
  RegistrationListener(const EventSourcePtr& registration);

  void handleEvent(RegistryEventPtr event) override;

private:
  void handleRegistration(const DevicePtr& device);
  void handleUpdate(const std::string& identifier);
  void handleDeregistration(const std::string& identifier);

  std::multimap<std::string, ObserverPtr> observers_;
};

}; // namespace LwM2M_Example

#endif //__LWM2M_EXAMPLE_APPLICATION_HPP