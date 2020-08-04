#ifndef __COAP_OPTION_URI_QUERY_DEFINITION_HPP
#define __COAP_OPTION_URI_QUERY_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <string>

namespace CoAP {
class UriQuery : public Option {
  std::string value_;

public:
  UriQuery();
  UriQuery(std::string value);

  std::vector<uint8_t> getValue() override;
  std::string getAsString() override;
};

} // namespace CoAP

#endif //__COAP_OPTION_URI_QUERY_DEFINITION_HPP