#include "CoAP_Binding.hpp"
#include "Variant_Visitor.hpp"

using namespace std;
using namespace CoAP;

namespace LwM2M {

CoAP_Binding::CoAP_Binding(CoAP_BindingConfigPtr config,
                           RequestsManagerInterfacePtr requests_manager,
                           DeviceRegistryPtr registry)
    : BindingInterface(requests_manager, registry) {
  match(config->address_,
        [&](bool ipv6_flag) {
          socket_ = make_shared<Server>(ipv6_flag, config->port_);
        },
        [&](string address) {
          socket_ = make_shared<Server>(address, config->port_);
        });
}

void CoAP_Binding::start() {
  socket_->start();
  Stoppable::start();
}

void CoAP_Binding::run() {
  while (!stopRequested()) {
  }
}

} // namespace LwM2M
