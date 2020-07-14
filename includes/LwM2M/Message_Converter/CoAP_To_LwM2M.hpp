#ifndef __COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP
#define __COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP

#include "CoAP_Message.hpp"
#include "Converter.hpp"
#include "LwM2M_Message.hpp"
#include "Threadsafe_Queue.hpp"

namespace LwM2M_Model {
class CoAP_To_LwM2M : public Converter<CoAP::CoAP_Message> {
  std::shared_ptr<ThreadsafeQueue<LwM2M_Model::LwM2M_Message>> output_queue_;

public:
  CoAP_To_LwM2M(std::shared_ptr<ThreadsafeQueue<LwM2M_Model::LwM2M_Message>>
                    output_queue);

  void convert(std::shared_ptr<CoAP::CoAP_Message> message) override;
};
} // namespace LwM2M_Model

#endif //__COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP