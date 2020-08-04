#ifndef __COAP_OPTION_MAX_AGE_DEFINITION_HPP
#define __COAP_OPTION_MAX_AGE_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <cstdint>
#include <string>

namespace CoAP {
class MaxAge : public Option {
  uint64_t value_;

public:
  MaxAge();
  MaxAge(uint8_t value);
  MaxAge(uint16_t value);
  MaxAge(uint32_t value);
  MaxAge(uint64_t value);
  MaxAge(std::vector<uint8_t> value);

  std::vector<uint8_t> getValue() override;
  std::string getAsString() override;
  uint64_t getAsLong();
};
} // namespace CoAP

#endif //__COAP_OPTION_MAX_AGE_DEFINITION_HPP