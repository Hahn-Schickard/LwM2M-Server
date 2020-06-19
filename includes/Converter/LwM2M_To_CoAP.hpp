#ifndef _LWM2M_MESSAGE_TO_COAP_MESAGE_CONVERTER_HPP
#define _LWM2M_MESSAGE_TO_COAP_MESAGE_CONVERTER_HPP

#include "CoAP_Message.hpp"
#include "Converter.hpp"
#include "LwM2M_Message.hpp"
#include "Threadsafe_Queue.hpp"

namespace LwM2M_Model {
class LwM2M_To_CoAP : public Converter<LwM2M_Message> {
  std::shared_ptr<ThreadsafeQueue<CoAP::CoAP_Message>> output_queue_;

public:
  LwM2M_To_CoAP(
      std::shared_ptr<ThreadsafeQueue<CoAP::CoAP_Message>> output_queue);

  void convert(std::shared_ptr<LwM2M_Message> message) override;
};
} // namespace LwM2M_Model
#endif //_LWM2M_MESSAGE_TO_COAP_MESAGE_CONVERTER_HPP