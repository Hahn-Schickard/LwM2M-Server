#ifndef __LWM2M_EXAMPLE_APPLICATION_HPP
#define __LWM2M_EXAMPLE_APPLICATION_HPP

#include "DeviceRegistry.hpp"
#include "Event_Model/EventListenerInterface.hpp"
#include "RegistryEvent.hpp"
#include "Variant_Visitor.hpp"

namespace LwM2M_Example {
using namespace LwM2M;

class RegistrationListener
    : public Event_Model::EventListenerInterface<RegistryEvent> {
public:
  RegistrationListener(EventSourcePtr registration);

  void handleEvent(RegistryEventPtr event) override;
};

}; // namespace LwM2M_Example

#endif //__LWM2M_EXAMPLE_APPLICATION_HPP