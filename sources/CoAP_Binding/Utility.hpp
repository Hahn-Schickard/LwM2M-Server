#ifndef __LWM2M_COAP_BINDING_UTILITY_HPP
#define __LWM2M_COAP_BINDING_UTILITY_HPP

#include "CoAP/Message.hpp"

namespace LwM2M {

inline size_t generateHash(CoAP::MessagePtr message) {
  return message->getHeader()->getMessageID() << message->getToken().size() |
         message->getTokenHash();
}
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_UTILITY_HPP