#ifndef __LWM2M_COAP_BINDING_HPP
#define __LWM2M_COAP_BINDING_HPP

#include "Binding.hpp"
#include "CoAP/Socket.hpp"
#include "CoAP_Config.hpp"
#include "Logger.hpp"

namespace LwM2M {

class CoAP_Binding : public BindingInterface, public CoAP::Socket {
  std::shared_ptr<HaSLL::Logger> logger_;

  ClientResponsePtr makeClientResponse(CoAP::MessagePtr message);
  CoAP::MessagePtr encode(CoAP::MessagePtr request, ServerResponsePtr message);

  void handleResponse(CoAP::MessagePtr message);
  void handleNotification(CoAP::MessagePtr message);
  ServerResponsePtr handleRegistrationRequest(CoAP::MessagePtr message);
  ServerResponsePtr handleRequest(CoAP::MessagePtr message);

  void handleReceived(CoAP::MessagePtr message) override final;

public:
  CoAP_Binding(DeviceRegistryPtr registry, const CoAP_Config &config);
  ~CoAP_Binding();

  void start() override final;
  void stop() override final;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_HPP