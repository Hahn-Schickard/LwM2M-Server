#ifndef __LWM2M_BINDING_INTERFACE_HPP
#define __LWM2M_BINDING_INTERFACE_HPP

#include "Registrator.hpp"
#include "Requester.hpp"
#include "Stoppable.hpp"

namespace LwM2M {

class BindingInterface : public Stoppable {
protected:
  RequesterPtr requester_;
  RegistratorPtr registrator_;

public:
  BindingInterface(RequesterPtr requester, DeviceRegistryPtr registry)
      : requester_(requester),
        registrator_(std::make_shared<Registrator>(registry, requester_)) {}
};

} // namespace LwM2M

#endif //__LWM2M_BINDING_INTERFACE_HPP