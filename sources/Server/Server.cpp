#include "Server.hpp"
#include "CoAP_Binding.hpp"
#include "Config.hpp"

#include "HaSLL/LoggerManager.hpp"

using namespace std;
using namespace HaSLL;

namespace LwM2M {

Server::Server(const string& filepath)
    : logger_(LoggerManager::registerTypedLogger(this)) {
  auto config = getConfig(filepath);
  registry_ = make_shared<DeviceRegistry>(config.descriptors_);

  for (const auto& binding : config.bindings_) {
    switch (binding.first) {
    case ServerBindingType::CoAP: {
      auto coap_binding = make_unique<CoAP_Binding>(registry_, binding.second);
      bindings_.emplace_back(move(coap_binding));
      break;
    }
    default: {
      break;
    }
    }
  }
}

void Server::start() {
  logger_->trace("Starting bindings.");
  for (const auto& binding : bindings_) {
    binding->start();
  }
  logger_->trace("All registered bindings have been started.");
}

vector<string> Server::getRegisterBindingNames() {
  vector<string> result;
  result.reserve(bindings_.size());
  for (const auto& binding : bindings_) {
    result.push_back(binding->name());
  }
  return result;
}

void Server::stop() {
  logger_->trace("Stopping bindings.");
  for (const auto& binding : bindings_) {
    binding->stop();
  }
  logger_->trace("All registered bindings have been stopped.");
}

EventSourcePtr Server::getEventSource() { return registry_; }

} // namespace LwM2M
