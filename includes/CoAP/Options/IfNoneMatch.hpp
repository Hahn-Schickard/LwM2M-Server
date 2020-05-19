#ifndef __COAP_OPTION_IF_NONE_MATCH_DEFINITION_HPP
#define __COAP_OPTION_IF_NONE_MATCH_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <string>

namespace CoAP {
class IfNoneMatch : public CoAP_Option {
public:
  IfNoneMatch();

  std::string getValue() override;
};

} // namespace CoAP

#endif //__COAP_OPTION_IF_NONE_MATCH_DEFINITION_HPP