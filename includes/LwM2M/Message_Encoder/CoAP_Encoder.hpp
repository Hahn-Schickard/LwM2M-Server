#ifndef __LWM2M_COAP_MESSAGE_ENCODER_HPP
#define __LWM2M_COAP_MESSAGE_ENCODER_HPP

#include "CoAP_Message.hpp"
#include "Logger.hpp"
#include "Message_Encoder.hpp"
#include "RegistrationMessages.hpp"
#include "Threadsafe_HashSet.hpp"

namespace LwM2M {

class CoAP_Encoder : public MessageEncoder {
  std::shared_ptr<ThreadsafeHashSet<CoAP::Message>> output_queue_;
  std::shared_ptr<HaSLL::Logger> logger_;

public:
  CoAP_Encoder(std::shared_ptr<ResponseHandler> response_handler,
               std::shared_ptr<ThreadsafeHashSet<CoAP::Message>> output_queue);

  ResponseFuture encode(std::unique_ptr<ServerRequest_Read> input) override;

  void encode(std::unique_ptr<ServerResponse_Register> input) override;
  void encode(std::unique_ptr<Response> input) override;
};
} // namespace LwM2M
#endif //__LWM2M_COAP_MESSAGE_ENCODER_HPP