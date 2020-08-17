#ifndef __LWM2M_COAP_MESSAGE_ENCODER_HPP
#define __LWM2M_COAP_MESSAGE_ENCODER_HPP

#include "CoAP_Message.hpp"
#include "Logger.hpp"
#include "Message_Encoder.hpp"
#include "Threadsafe_Unique_Queue.hpp"

namespace LwM2M {

class CoAP_Encoder : public MessageEncoder {
  std::shared_ptr<ThreadsafeUniqueQueue<CoAP::Message>> output_queue_;
  std::shared_ptr<HaSLL::Logger> logger_;

public:
  CoAP_Encoder(
      std::shared_ptr<ThreadsafeUniqueQueue<CoAP::Message>> output_queue);

  void encode(std::unique_ptr<Read_Request> input) override;

  void encode(std::unique_ptr<Register_Response> input) override;
  void encode(std::unique_ptr<Response> input) override;
};
} // namespace LwM2M
#endif //__LWM2M_COAP_MESSAGE_ENCODER_HPP