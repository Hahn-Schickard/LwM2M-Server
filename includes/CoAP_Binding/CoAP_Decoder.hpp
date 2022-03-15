#ifndef __LWM2M_COAP_DECODER_HPP
#define __LWM2M_COAP_DECODER_HPP

#include "CoAPS4Cpp/Message.hpp"
#include "Deregister.hpp"
#include "Logger.hpp"
#include "Message.hpp"
#include "Register.hpp"
#include "Update.hpp"

namespace LwM2M {
struct CoAP_Decoder {
  CoAP_Decoder();
  ~CoAP_Decoder();

  template <class LwM2M_MessageType>
  std::shared_ptr<LwM2M_MessageType> decode(CoAP::MessagePtr message) {
    throw std::runtime_error(
        "Decoder for a given message type is not supported");
  }

private:
  LwM2M::PayloadPtr decode(CoAP::ContentFormatPtr content_format,
                           CoAP::PayloadPtr payload);

  std::shared_ptr<HaSLL::Logger> logger_;
};

template <>
ClientResponsePtr
CoAP_Decoder::decode<ClientResponse>(CoAP::MessagePtr message);
template <>
RegisterRequestPtr
CoAP_Decoder::decode<RegisterRequest>(CoAP::MessagePtr message);
template <>
UpdateRequestPtr CoAP_Decoder::decode<UpdateRequest>(CoAP::MessagePtr message);
template <>
DeregisterRequestPtr
CoAP_Decoder::decode<DeregisterRequest>(CoAP::MessagePtr message);
} // namespace LwM2M

#endif //__LWM2M_COAP_DECODER_HPP