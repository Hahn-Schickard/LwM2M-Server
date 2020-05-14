#ifndef __LWM2M_CONVERTER_HPP
#define __LWM2M_CONVERTER_HPP

#include "CoAP_Message.hpp"
#include "LwM2M_Message.hpp"

namespace LwM2M_Model {

LwM2M_Message &convert(CoAP::CoAP_Message input);
CoAP::CoAP_Message &convert(LwM2M_Message input);

} // namespace LwM2M_Model

#endif //__LWM2M_CONVERTER_HPP