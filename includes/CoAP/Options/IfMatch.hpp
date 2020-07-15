#ifndef __COAP_OPTION_IF_MATCH_DEFINITION_HPP
#define __COAP_OPTION_IF_MATCH_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <string>

namespace CoAP {
class IfMatch : public Option {
  std::string value_; // opaque type

public:
  IfMatch();
  IfMatch(std::string value);

  std::string getValue() override;
};
} // namespace CoAP
#endif //__COAP_OPTION_IF_MATCH_DEFINITION_HPP