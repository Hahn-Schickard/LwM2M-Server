#ifndef __LWM2M_COAP_ENCODER_HPP
#define __LWM2M_COAP_ENCODER_HPP

#include "DeviceManagmentMessage.hpp"
#include "InformationReporingMessage.hpp"

#include "CoAPS4Cpp/Message.hpp"
#include "HaSLL/Logger.hpp"

namespace LwM2M {
struct CoAP_Encoder {
  CoAP_Encoder();
  ~CoAP_Encoder();

  CoAP::MessagePtr encode(const DeviceManagementRequestPtr& message);
  CoAP::MessagePtr encode(const InformationReportingRequestPtr& message);
  CoAP::MessagePtr encode(const ServerRequestPtr& request);
  CoAP::MessagePtr encode(
      const CoAP::MessagePtr& request, const ServerResponsePtr& response);

private:
  CoAP::PayloadPtr encode(LwM2M::MessageType type,
      const LwM2M::PayloadPtr& payload, const std::string& message_identifier);
  CoAP::Options makeOptions(LwM2M::MessageType type,
      const LwM2M::PayloadPtr& payload, const std::string& message_identifier);
  CoAP::Options makeOptions(
      const LwM2M::PayloadPtr& payload, const std::string& message_identifier);

  HaSLL::LoggerPtr logger_;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_ENCODER_HPP