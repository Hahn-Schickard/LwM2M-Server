#ifndef __COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP
#define __COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP

#include "CoAP_Message.hpp"
#include "Converter.hpp"
#include "LwM2M_Message.hpp"
#include "Threadsafe_Queue.hpp"

namespace LwM2M {
class CoAP_To_LwM2M : public Converter<CoAP::Message> {
  std::shared_ptr<ThreadsafeQueue<Message>> output_queue_;

public:
  CoAP_To_LwM2M(std::shared_ptr<ThreadsafeQueue<Message>> output_queue);

  void convert(std::unique_ptr<CoAP::Message> message) override;
};
} // namespace LwM2M

#endif //__COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP