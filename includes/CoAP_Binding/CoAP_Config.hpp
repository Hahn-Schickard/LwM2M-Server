#ifndef __LWM2M_COAP_BINDING_CONFIGURATION_HPP
#define __LWM2M_COAP_BINDING_CONFIGURATION_HPP

#include <string>

namespace LwM2M {
struct CoAP_Config {
  std::string address_ = "0.0.0.0";
  unsigned int port_ = 5683;
};

CoAP_Config getCoAP_Config(const std::string filepath);
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_CONFIGURATION_HPP