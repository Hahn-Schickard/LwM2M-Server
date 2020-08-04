#ifndef __COAP_OPTION_ACCEPT_DEFINITION_HPP
#define __COAP_OPTION_ACCEPT_DEFINITION_HPP

#include "CoAP_Option.hpp"
#include "ContentFormat.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace CoAP {
class Accept : public Option {
  ContentFormatType value_;

public:
  Accept();
  Accept(ContentFormatType format_type);
  Accept(std::vector<uint8_t> value);

  std::vector<uint8_t> getValue() override;
  std::string getAsString() override;
  uint16_t getAsShort();
  ContentFormatType getAcceptableContentFormatType();
};
} // namespace CoAP

#endif //__COAP_OPTION_ACCEPT_DEFINITION_HPP