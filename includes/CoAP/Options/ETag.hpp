#ifndef __COAP_OPTION_ETAG_DEFINITION_HPP
#define __COAP_OPTION_ETAG_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <string>

namespace CoAP {
class ETag : public Option {
  std::string value_; // opaque type

public:
  ETag();
  ETag(std::string value);

  std::vector<uint8_t> getValue() override;
  std::string getAsString() override;
};

} // namespace CoAP

#endif //__COAP_OPTION_ETAG_DEFINITION_HPP