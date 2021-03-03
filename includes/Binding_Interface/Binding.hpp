#ifndef __LWM2M_BINDING_INTERFACE_HPP
#define __LWM2M_BINDING_INTERFACE_HPP

#include "Registrator.hpp"
#include "RequestsManagerInterface.hpp"
#include "Stoppable.hpp"

namespace LwM2M {

class BindingInterface : public Stoppable {
protected:
  ResponseHandlerPtr response_handler_ = std::make_shared<ResponseHandler>();
  RequestsManagerInterfacePtr requests_manager_;
  RegistratorPtr registrator_;

  void bind(RequestsManagerInterfacePtr requests_manager,
            DeviceRegistryPtr registry) {
    requests_manager_ = requests_manager;
    registrator_ = std::make_shared<Registrator>(registry, requests_manager_);
  }
};

using BindingInterfacePtr = std::shared_ptr<BindingInterface>;

} // namespace LwM2M

#endif //__LWM2M_BINDING_INTERFACE_HPP