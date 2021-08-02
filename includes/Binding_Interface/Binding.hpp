#ifndef __LWM2M_BINDING_INTERFACE_HPP
#define __LWM2M_BINDING_INTERFACE_HPP

#include "Registrator.hpp"

namespace LwM2M {

class BindingInterface {
  DeviceRegistryPtr registry_;

protected:
  BindingInterface(DeviceRegistryPtr registry) : registry_(registry) {}

public:
  virtual void start() = 0;
  virtual void stop() = 0;
};

using BindingInterfacePtr = std::shared_ptr<BindingInterface>;

} // namespace LwM2M

#endif //__LWM2M_BINDING_INTERFACE_HPP