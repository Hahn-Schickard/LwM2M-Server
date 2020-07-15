#ifndef __COAP_OPTION_PROXY_SCHEME_DEFINITION_HPP
#define __COAP_OPTION_PROXY_SCHEME_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <string>

namespace CoAP {
class ProxyScheme : public Option {
  std::string value_;

public:
  ProxyScheme();
  ProxyScheme(std::string value);

  std::string getValue() override;
};
} // namespace CoAP

#endif //__COAP_OPTION_PROXY_SCHEME_DEFINITION_HPP