#ifndef __LWM2M_COAP_BINDING_HPP
#define __LWM2M_COAP_BINDING_HPP

#include "Binding.hpp"
#include "CoAP/Socket.hpp"

#include <variant>

namespace LwM2M {
struct CoAP_BindingConfig {
  const std::variant<bool, std::string> address_ = false;
  const unsigned int port_ = 5683;

  CoAP_BindingConfig() = default;
  CoAP_BindingConfig(bool local_ipv6_address) : address_(local_ipv6_address) {}
  CoAP_BindingConfig(std::string string_address) : address_(string_address) {}
  CoAP_BindingConfig(unsigned int port) : port_(port) {}
  CoAP_BindingConfig(bool local_ipv6_address, unsigned int port)
      : address_(local_ipv6_address), port_(port) {}
  CoAP_BindingConfig(std::string address, unsigned int port)
      : address_(address), port_(port) {}
};

using CoAP_BindingConfigPtr = std::shared_ptr<CoAP_BindingConfig>;

class CoAP_Binding : public BindingInterface {
  CoAP::SocketPtr socket_;
  CoAP::InboxPtr inbox_;

  CoAP::MessagePtr handleResponse(CoAP::MessagePtr message);
  CoAP::MessagePtr handleNotification(CoAP::MessagePtr message);
  ServerResponsePtr handleRegistrationRequest(CoAP::MessagePtr message);
  ServerResponsePtr handleRequest(CoAP::MessagePtr message);
  CoAP::MessagePtr handleMessage(CoAP::MessagePtr message);

public:
  CoAP_Binding(DeviceRegistryPtr registry,
               CoAP_BindingConfigPtr config = CoAP_BindingConfigPtr());

  void start();

  void run() override final;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_HPP