#ifndef _LWM2M_MESSAGE_TO_COAP_MESAGE_CONVERTER_HPP
#define _LWM2M_MESSAGE_TO_COAP_MESAGE_CONVERTER_HPP

#include "CoAP_Message.hpp"
#include "Converter.hpp"
#include "LwM2M_Message.hpp"
#include "Queue_Interface.hpp"

namespace LwM2M {
class LwM2M_To_CoAP : public Converter<Message> {
  std::shared_ptr<QueueInterface<CoAP::Message>> output_queue_;

public:
  LwM2M_To_CoAP(std::shared_ptr<QueueInterface<CoAP::Message>> output_queue);

  void convert(std::unique_ptr<Message> message) override;
};
} // namespace LwM2M
#endif //_LWM2M_MESSAGE_TO_COAP_MESAGE_CONVERTER_HPP