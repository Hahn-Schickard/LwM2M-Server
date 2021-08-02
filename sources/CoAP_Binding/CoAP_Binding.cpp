#include "CoAP_Binding.hpp"

using namespace std;

namespace LwM2M {
CoAP_Binding::CoAP_Binding(DeviceRegistryPtr registry,
                           const std::string &config_filepath)
    : CoAP_Binding(registry, getCoAP_Config(config_filepath)) {}

CoAP_Binding::CoAP_Binding(DeviceRegistryPtr registry,
                           const CoAP_Config &config)
    : BindingInterface(registry),
      server_(make_shared<CoAP_Server>(registry, config)) {}

void CoAP_Binding::start() { server_->start(); }
void CoAP_Binding::stop() { server_->stop(); }

} // namespace LwM2M
