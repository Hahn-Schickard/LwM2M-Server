#ifndef __LWM2M_COAP_BINDING_HPP
#define __LWM2M_COAP_BINDING_HPP

#include "Binding.hpp"
#include "CoAP_Decoder.hpp"
#include "CoAP_Encoder.hpp"
#include "Registrator.hpp"

#include "CoAPS4Cpp/Socket.hpp"
#include "HaSLL/Logger.hpp"

namespace LwM2M {

struct CoAP_Binding : public BindingInterface,
                      public Registrator,
                      public CoAP::Socket {
  CoAP_Binding(
      const DeviceRegistryPtr& registry, const std::string& config_filepath);
  CoAP_Binding(
      const DeviceRegistryPtr& registry, const CoAP::Configuration& config);

  ~CoAP_Binding();

  // Requester interface implementation
  std::future<DataFormatPtr> requestData(
      const DeviceManagementRequestPtr& request) final;
  std::future<TargetContentVector> requestMultiTargetData(
      const DeviceManagementRequestPtr& request) final;
  std::future<bool> requestAction(
      const DeviceManagementRequestPtr& request) final;
  std::future<ClientResponsePtr> request(const ServerRequestPtr& request) final;
  size_t requestObservation(
      const std::function<void(PayloadDataPtr)>& notify_cb,
      const InformationReportingRequestPtr& request) final;
  void cancelObservation(
      size_t observer_id, const InformationReportingRequestPtr& request) final;
  void cancelRequest(const ServerRequestPtr& request) final;

  void start();
  void stop();

private:
  void handleNotification(const CoAP::MessagePtr& message);
  ServerResponsePtr handleRegistrationRequest(const CoAP::MessagePtr& message);
  ServerResponsePtr handleRequest(const CoAP::MessagePtr& message);

  // Socket interface implementation
  void handleReceived(const CoAP::MessagePtr& message) final;

  // only encodes the request if it was NOT dispatched before, otherwise returns
  // the dispatched encoded request
  CoAP::MessagePtr encodeRequest(const ServerRequestPtr& request);

  std::unique_ptr<CoAP_Encoder> encoder_;
  std::unique_ptr<CoAP_Decoder> decoder_;
  HaSLL::LoggerPtr logger_;
  std::unordered_map<std::size_t, CoAP::MessagePtr> dispatched_;
  std::unordered_map<std::size_t, std::function<void(PayloadDataPtr)>>
      observed_elements_;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_HPP