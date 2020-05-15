#ifndef __LWM2M_CONVERTER_HPP
#define __LWM2M_CONVERTER_HPP

#include "CoAP_Message.hpp"
#include "LwM2M_Message.hpp"

#include <memory>

namespace LwM2M_Model {

std::unique_ptr<LwM2M_Message>
convert(std::shared_ptr<CoAP::CoAP_Message> input);
CoAP::CoAP_Message &convert(std::unique_ptr<LwM2M_Message> input);

} // namespace LwM2M_Model

#endif //__LWM2M_CONVERTER_HPP