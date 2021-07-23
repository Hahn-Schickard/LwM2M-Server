#ifndef __LWM2M_COAP_SERVER_HPP
#define __LWM2M_COAP_SERVER_HPP

#include "CoAP/Socket.hpp"
#include "CoAP_Config.hpp"
#include "Logger.hpp"
#include "Registrator.hpp"

namespace LwM2M {

struct CoAP_Server : public Registrator, public CoAP::Socket {
  CoAP_Server(DeviceRegistryPtr registry, const CoAP_Config &config);
  ~CoAP_Server();

  // Requester interface implementation
  std::future<DataFormatPtr>
  requestData(ServerRequestPtr message) override final;
  std::future<TargetContentVector>
  requestMultiTargetData(ServerRequestPtr message) override final;
  std::future<bool> requestAction(ServerRequestPtr message) override final;
  std::future<ClientResponsePtr>
  request(ServerRequestPtr message) override final;

  // Socket interface implementation
  void handleReceived(CoAP::MessagePtr message) override final;

  void start();
  void stop();

private:
  ClientResponsePtr makeClientResponse(CoAP::MessagePtr message);
  void handleNotification(CoAP::MessagePtr message);
  ServerResponsePtr handleRegistrationRequest(CoAP::MessagePtr message);
  ServerResponsePtr handleRequest(CoAP::MessagePtr message);

  std::shared_ptr<HaSLL::Logger> logger_;
};

using CoAP_ServerPtr = std::shared_ptr<CoAP_Server>;
} // namespace LwM2M

#endif //__LWM2M_COAP_SERVER_HPP