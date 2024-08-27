#ifndef __LWM2M_COAP_DECODER_HPP
#define __LWM2M_COAP_DECODER_HPP

#include "Deregister.hpp"
#include "Message.hpp"
#include "Register.hpp"
#include "Update.hpp"

#include "CoAPS4Cpp/Message.hpp"
#include "HaSLL/Logger.hpp"

namespace LwM2M {
struct CoAP_Decoder {
  CoAP_Decoder();
  ~CoAP_Decoder();

  template <class LwM2M_MessageType>
  std::shared_ptr<LwM2M_MessageType> decode(const CoAP::MessagePtr& message) {
    throw std::runtime_error(
        "Decoder for a given message type is not supported");
  }

private:
  LwM2M::PayloadPtr decode(const CoAP::ContentFormatPtr& content_format,
      const CoAP::PayloadPtr& payload);

  HaSLL::LoggerPtr logger_;
};

template <>
ClientResponsePtr CoAP_Decoder::decode<ClientResponse>(
    const CoAP::MessagePtr& message);
template <>
RegisterRequestPtr CoAP_Decoder::decode<RegisterRequest>(
    const CoAP::MessagePtr& message);
template <>
UpdateRequestPtr CoAP_Decoder::decode<UpdateRequest>(
    const CoAP::MessagePtr& message);
template <>
DeregisterRequestPtr CoAP_Decoder::decode<DeregisterRequest>(
    const CoAP::MessagePtr& message);
} // namespace LwM2M

#endif //__LWM2M_COAP_DECODER_HPP