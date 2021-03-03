#ifndef __LWM2M_COAP_BINDING_HPP
#define __LWM2M_COAP_BINDING_HPP

#include "Binding.hpp"
#include "CoAP/Socket.hpp"
#include "Logger.hpp"

namespace LwM2M {

class CoAP_Binding : public BindingInterface {
  CoAP::SocketPtr socket_;
  CoAP::InboxPtr inbox_;
  std::shared_ptr<HaSLL::Logger> logger_;

  CoAP::MessagePtr handleResponse(CoAP::MessagePtr message);
  CoAP::MessagePtr handleNotification(CoAP::MessagePtr message);
  ServerResponsePtr handleRegistrationRequest(CoAP::MessagePtr message);
  ServerResponsePtr handleRequest(CoAP::MessagePtr message);
  CoAP::MessagePtr handleMessage(CoAP::MessagePtr message);

public:
  CoAP_Binding(DeviceRegistryPtr registry,
               const std::string filepath = std::string());
  ~CoAP_Binding();

  void run() override final;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_HPP