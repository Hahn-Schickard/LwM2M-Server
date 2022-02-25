#ifndef __LWM2M_COAP_ENCODER_HPP
#define __LWM2M_COAP_ENCODER_HPP

#include "CoAP/Message.hpp"
#include "DeviceManagmentMessage.hpp"
#include "InformationReporingMessage.hpp"
#include "Logger.hpp"

namespace LwM2M {
struct CoAP_Encoder {
  CoAP_Encoder();
  ~CoAP_Encoder();

  CoAP::MessagePtr encode(DeviceManagementRequestPtr message);
  CoAP::MessagePtr encode(InformationReportingRequestPtr message);
  CoAP::MessagePtr encode(ServerRequestPtr message);
  CoAP::MessagePtr encode(CoAP::MessagePtr request, ServerResponsePtr response);

private:
  // encoding means taking one type, in this instance LwM2M::PayloadPtr  and
  // converting it into another, here it is CoAP::PayloadPtr
  CoAP::PayloadPtr encode(LwM2M::MessageType type, LwM2M::PayloadPtr payload);

  // makeing an option means that we take some information, like the message
  // type and the actual payload type and create new information from it
  CoAP::Options makeOptions(LwM2M::MessageType type, LwM2M::PayloadPtr payload);
  CoAP::Options makeOptions(LwM2M::PayloadPtr payload);

  std::shared_ptr<HaSLL::Logger> logger_;
};
} // namespace LwM2M

#endif //__LWM2M_COAP_ENCODER_HPP