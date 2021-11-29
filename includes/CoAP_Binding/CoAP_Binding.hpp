#ifndef __LWM2M_COAP_BINDING_HPP
#define __LWM2M_COAP_BINDING_HPP

#include "Binding.hpp"
#include "CoAP/Socket.hpp"
#include "Logger.hpp"
#include "Registrator.hpp"

namespace LwM2M {

struct CoAP_Binding : public BindingInterface,
                      public Registrator,
                      public CoAP::Socket {
  CoAP_Binding(DeviceRegistryPtr registry, const std::string &config_filepath);
  CoAP_Binding(DeviceRegistryPtr registry, const CoAP::Configuration &config);

  ~CoAP_Binding();

  // Requester interface implementation
  std::future<DataFormatPtr>
  requestData(DeviceManagementRequestPtr message) override final;
  std::future<TargetContentVector>
  requestMultiTargetData(DeviceManagementRequestPtr message) override final;
  std::future<bool>
  requestAction(DeviceManagementRequestPtr message) override final;
  std::future<ClientResponsePtr>
  request(ServerRequestPtr message) override final;
  size_t
  requestObservation(std::function<void(DataFormatPtr)> notify_cb,
                     InformationReportingRequestPtr message) override final;
  void cancelObservation(size_t observer_id,
                         InformationReportingRequestPtr message) override final;
  void cancelRequest(ServerRequestPtr message) override final;

  void start();
  void stop();

private:
  ClientResponsePtr makeClientResponse(CoAP::MessagePtr message);
  void handleNotification(CoAP::MessagePtr message);
  ServerResponsePtr handleRegistrationRequest(CoAP::MessagePtr message);
  ServerResponsePtr handleRequest(CoAP::MessagePtr message);

  // Socket interface implementation
  void handleReceived(CoAP::MessagePtr message) override final;

  CoAP::MessagePtr encodeRequest(ServerRequestPtr request);

  std::shared_ptr<HaSLL::Logger> logger_;
  std::unordered_map<std::size_t, CoAP::MessagePtr> dispatched_;
  std::unordered_map<std::size_t, std::function<void(DataFormatPtr)>>
      observed_elements_;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_HPP