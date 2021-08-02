#ifndef __LWM2M_COAP_BINDING_HPP
#define __LWM2M_COAP_BINDING_HPP

#include "Binding.hpp"
#include "CoAP/Socket.hpp"
#include "CoAP_Config.hpp"
#include "Logger.hpp"
#include "Registrator.hpp"

namespace LwM2M {

struct CoAP_Binding : public BindingInterface,
                      public Registrator,
                      public CoAP::Socket {
  CoAP_Binding(DeviceRegistryPtr registry, const std::string &config_filepath);
  CoAP_Binding(DeviceRegistryPtr registry, const CoAP_Config &config);

  ~CoAP_Binding();

  // Requester interface implementation
  std::future<DataFormatPtr>
  requestData(ServerRequestPtr message) override final;
  std::future<TargetContentVector>
  requestMultiTargetData(ServerRequestPtr message) override final;
  std::future<bool> requestAction(ServerRequestPtr message) override final;
  std::future<ClientResponsePtr>
  request(ServerRequestPtr message) override final;

  void start();
  void stop();

private:
  ClientResponsePtr makeClientResponse(CoAP::MessagePtr message);
  void handleNotification(CoAP::MessagePtr message);
  ServerResponsePtr handleRegistrationRequest(CoAP::MessagePtr message);
  ServerResponsePtr handleRequest(CoAP::MessagePtr message);

  // Socket interface implementation
  void handleReceived(CoAP::MessagePtr message) override final;

  std::shared_ptr<HaSLL::Logger> logger_;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_HPP