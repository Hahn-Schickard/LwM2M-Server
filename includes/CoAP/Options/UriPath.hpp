#ifndef __COAP_OPTION_URI_PATH_DEFINITION_HPP
#define __COAP_OPTION_URI_PATH_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <string>

namespace CoAP {
class UriPath : public Option {
  std::string value_;

public:
  UriPath();
  UriPath(std::string value);

  std::string getValue() override;
};

} // namespace CoAP

#endif //__COAP_OPTION_URI_PATH_DEFINITION_HPP