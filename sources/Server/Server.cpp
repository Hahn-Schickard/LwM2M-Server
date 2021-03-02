#include "Server.hpp"
#include "CoAP_Binding.hpp"
#include "Config.hpp"

using namespace std;

namespace LwM2M {

Server::Server(const string filepath) {
  Configuration config;
  if (!filepath.empty()) {
    config = getConfig(filepath);
  }
  registry_ = make_shared<DeviceRegistry>(config.model_descriptors_);

  for (auto binding : config.bindings_) {
    switch (binding.first) {
    case ServerBindingType::CoAP: {
      auto coap_binding = make_unique<CoAP_Binding>(registry_);
      bindings_.emplace_back(move(coap_binding), "CoAP Binding");
      break;
    }
    default: { break; }
    }
  }
}

void Server::start() {
  for (auto &binding : bindings_) {
    binding.startTask();
  }
}

void Server::stop() {
  for (auto &binding : bindings_) {
    binding.stopTask();
  }
}

EventSourcePtr Server::getEventSource() { return registry_; }

} // namespace LwM2M
