#ifndef __COAP_OPTION_URI_HOST_DEFINITION_HPP
#define __COAP_OPTION_URI_HOST_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <string>

namespace CoAP {
class UriHost : public CoAP_Option {
  std::string value_;

public:
  UriHost();
  UriHost(std::string value);

  std::string getValue() override;
};
} // namespace CoAP
#endif //__COAP_OPTION_URI_HOST_DEFINITION_HPP