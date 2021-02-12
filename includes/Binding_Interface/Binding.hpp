#ifndef __LWM2M_BINDING_INTERFACE_HPP
#define __LWM2M_BINDING_INTERFACE_HPP

#include "Dispatcher.hpp"
#include "Registrator.hpp"
#include "Stoppable.hpp"

namespace LwM2M {

class BindingInterface : public Stoppable {
protected:
  DispatcherInterfacePtr dispatcher_;
  RegistratorPtr registrator_;

public:
  BindingInterface(DispatcherInterfacePtr dispatcher,
                   DeviceRegistryPtr registry)
      : dispatcher_(dispatcher),
        registrator_(std::make_shared<Registrator>(registry, dispatcher_)) {}
};

using BindingInterfacePtr = std::shared_ptr<BindingInterface>;

} // namespace LwM2M

#endif //__LWM2M_BINDING_INTERFACE_HPP