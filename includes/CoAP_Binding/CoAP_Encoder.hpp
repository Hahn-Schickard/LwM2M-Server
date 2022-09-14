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

  CoAP::MessagePtr encode(DeviceManagementRequestPtr message);
  CoAP::MessagePtr encode(InformationReportingRequestPtr message);
  CoAP::MessagePtr encode(ServerRequestPtr message);
  CoAP::MessagePtr encode(CoAP::MessagePtr request, ServerResponsePtr response);

private:
  CoAP::PayloadPtr encode(LwM2M::MessageType type, LwM2M::PayloadPtr payload,
      std::string message_identifier);
  CoAP::Options makeOptions(LwM2M::MessageType type, LwM2M::PayloadPtr payload,
      std::string message_identifier);
  CoAP::Options makeOptions(
      LwM2M::PayloadPtr payload, std::string message_identifier);

  HaSLI::LoggerPtr logger_;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_ENCODER_HPP