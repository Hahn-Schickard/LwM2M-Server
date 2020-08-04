#ifndef __COAP_OPTION_URI_PORT_DEFINITION_HPP
#define __COAP_OPTION_URI_PORT_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace CoAP {
class UriPort : public Option {
  uint16_t value_;

public:
  UriPort();
  UriPort(uint8_t value);
  UriPort(uint16_t value);
  UriPort(std::vector<uint8_t> value);

  std::vector<uint8_t> getValue() override;
  std::string getAsString() override;
  uint16_t getAsShort();
};
} // namespace CoAP

#endif //__COAP_OPTION_URI_PORT_DEFINITION_HPP