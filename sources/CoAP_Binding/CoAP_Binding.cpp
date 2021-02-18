#include "CoAP_Binding.hpp"
#include "CoAP_RequestsManager.hpp"
#include "Variant_Visitor.hpp"

using namespace std;
using namespace CoAP;

namespace LwM2M {

CoAP_Binding::CoAP_Binding(CoAP_BindingConfigPtr config,
                           DeviceRegistryPtr registry) {
  match(config->address_,
        [&](bool ipv6_flag) {
          socket_ = make_shared<Server>(ipv6_flag, config->port_);
        },
        [&](string address) {
          socket_ = make_shared<Server>(address, config->port_);
        });

  inbox_ = socket_->getInbox();

  BindingInterface::bind(make_shared<CoAP_RequestsManager>(
                             BindingInterface::response_handler_, socket_),
                         registry);
}

void CoAP_Binding::start() {
  socket_->start();
  Stoppable::start();
}

CoAP::MessagePtr CoAP_Binding::handleResponse(CoAP::MessagePtr message) {
  // get message id and token hash combo
  // create LwM2M::ClientResponse
  // call LwM2M::ResponseHandler::respond()
  return CoAP::MessagePtr();
}

CoAP::MessagePtr CoAP_Binding::handleNotification(CoAP::MessagePtr message) {
  // create LwM2M::ValueUpdated
  // forward to LwM2M::Notifier
  return CoAP::MessagePtr();
}

CoAP::MessagePtr CoAP_Binding::handleRequest(CoAP::MessagePtr message) {
  return CoAP::MessagePtr();
}

CoAP::MessagePtr CoAP_Binding::handleMessage(CoAP::MessagePtr message) {
  if (message->getHeader()->getMesageType() ==
      CoAP::MessageType::ACKNOWLEDGMENT) {
    return handleResponse(message);
  } else if (message->getHeader()->getMesageType() ==
             CoAP::MessageType::NON_CONFIRMABLE) {
    return handleNotification(message);
  } else if (message->getHeader()->getMesageType() ==
             CoAP::MessageType::CONFIRMABLE) {
    return handleRequest(message);
  } else {
    return CoAP::MessagePtr();
  }
}

void CoAP_Binding::run() {
  while (!stopRequested()) {
    auto message = inbox_->front();
    if (message) {
      auto response = handleMessage(message);
      if (response) {
        auto sent = socket_->send(response);
        sent.get();
      }
      inbox_->pop(message);
    }
  }
}

} // namespace LwM2M
