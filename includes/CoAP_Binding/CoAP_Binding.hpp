#ifndef __LWM2M_COAP_BINDING_HPP
#define __LWM2M_COAP_BINDING_HPP

#include "Binding.hpp"

#include "CoAP_Server.hpp"

namespace LwM2M {

class CoAP_Binding : public BindingInterface {
  CoAP_ServerPtr server_;

public:
  CoAP_Binding(DeviceRegistryPtr registry, const std::string &config_filepath);
  CoAP_Binding(DeviceRegistryPtr registry, const CoAP_Config &config);

  void start() override final;
  void stop() override final;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_HPP