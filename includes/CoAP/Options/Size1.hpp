#ifndef __COAP_OPTION_SIZE1_DEFINITION_HPP
#define __COAP_OPTION_SIZE1_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace CoAP {
class Size1 : public Option {
  uint64_t value_;

public:
  Size1();
  Size1(uint8_t value);
  Size1(uint16_t value);
  Size1(uint32_t value);
  Size1(uint64_t value);
  Size1(std::vector<uint8_t> value);

  std::vector<uint8_t> getValue() override;
  std::string getAsString() override;
  uint64_t getAsLong();
};

} // namespace CoAP

#endif //__COAP_OPTION_SIZE1_DEFINITION_HPP