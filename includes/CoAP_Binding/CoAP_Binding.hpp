#ifndef __LWM2M_COAP_BINDING_HPP
#define __LWM2M_COAP_BINDING_HPP

#include "Binding.hpp"
#include "CoAPS4Cpp/Socket.hpp"
#include "CoAP_Decoder.hpp"
#include "CoAP_Encoder.hpp"
#include "Logger.hpp"
#include "Registrator.hpp"

namespace LwM2M {

struct CoAP_Binding : public BindingInterface,
                      public Registrator,
                      public CoAP::Socket {
  CoAP_Binding(DeviceRegistryPtr registry, const std::string& config_filepath);
  CoAP_Binding(DeviceRegistryPtr registry, const CoAP::Configuration& config);

  ~CoAP_Binding();

  // Requester interface implementation
  std::future<DataFormatPtr> requestData(
      DeviceManagementRequestPtr message) override final;
  std::future<TargetContentVector> requestMultiTargetData(
      DeviceManagementRequestPtr message) override final;
  std::future<bool> requestAction(
      DeviceManagementRequestPtr message) override final;
  std::future<ClientResponsePtr> request(
      ServerRequestPtr message) override final;
  size_t requestObservation(std::function<void(PayloadDataPtr)> notify_cb,
      InformationReportingRequestPtr message) override final;
  void cancelObservation(size_t observer_id,
      InformationReportingRequestPtr message) override final;
  void cancelRequest(ServerRequestPtr message) override final;

  void start();
  void stop();

private:
  void handleNotification(CoAP::MessagePtr message);
  ServerResponsePtr handleRegistrationRequest(CoAP::MessagePtr message);
  ServerResponsePtr handleRequest(CoAP::MessagePtr message);

  // Socket interface implementation
  void handleReceived(CoAP::MessagePtr message) override final;

  // only encodes the request if it was NOT dispatched before, otherwise returns
  // the dispatched encoded request
  CoAP::MessagePtr encodeRequest(ServerRequestPtr request);

  std::unique_ptr<CoAP_Encoder> encoder_;
  std::unique_ptr<CoAP_Decoder> decoder_;
  std::shared_ptr<HaSLL::Logger> logger_;
  std::unordered_map<std::size_t, CoAP::MessagePtr> dispatched_;
  std::unordered_map<std::size_t, std::function<void(PayloadDataPtr)>>
      observed_elements_;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_HPP