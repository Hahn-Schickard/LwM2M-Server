#ifndef __COAP_OPTION_PROXY_URI_DEFINITION_HPP
#define __COAP_OPTION_PROXY_URI_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <string>

namespace CoAP {
class ProxyUri : public Option {
  std::string value_;

public:
  ProxyUri();
  ProxyUri(std::string value);

  std::string getValue() override;
};
} // namespace CoAP

#endif //__COAP_OPTION_PROXY_URI_DEFINITION_HPP