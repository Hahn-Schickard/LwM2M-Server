#ifndef __COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP
#define __COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP

#include "CoAP_Message.hpp"
#include "Converter.hpp"
#include "LwM2M_Message.hpp"

namespace LwM2M_Model {
class CoAP_To_LwM2M : public Converter<CoAP::CoAP_Message> {
  void convert(std::shared_ptr<CoAP::CoAP_Message> message) override;
};
} // namespace LwM2M_Model

#endif //__COAP_MESAGE_TO_LWM2M_MESSAGE_CONVERTER_HPP