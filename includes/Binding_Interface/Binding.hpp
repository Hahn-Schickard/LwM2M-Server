#ifndef __LWM2M_BINDING_INTERFACE_HPP
#define __LWM2M_BINDING_INTERFACE_HPP

#include "Registrator.hpp"

namespace LwM2M {

class BindingInterface {
  std::string name_;
  DeviceRegistryPtr registry_;

protected:
  BindingInterface(const std::string& name, DeviceRegistryPtr registry)
      : name_(name), registry_(registry) {}

public:
  virtual void start() = 0;
  virtual void stop() = 0;

  std::string name() { return name_; }
};

using BindingInterfacePtr = std::shared_ptr<BindingInterface>;

} // namespace LwM2M

#endif //__LWM2M_BINDING_INTERFACE_HPP