#include "Size1.hpp"
#include "PrimitiveConverter.hpp"

using namespace std;
using namespace utility;
namespace CoAP {
Size1::Size1() : Size1((uint8_t)0) {}

Size1::Size1(uint8_t value)
    : Option(OptionNumber::SIZE_1, 1, false, false, false, 4), value_(value) {}

Size1::Size1(uint16_t value)
    : Option(OptionNumber::SIZE_1, 2, false, false, false, 4), value_(value) {}

Size1::Size1(uint32_t value)
    : Option(OptionNumber::SIZE_1, 3, false, false, false, 4), value_(value) {}

Size1::Size1(uint64_t value)
    : Option(OptionNumber::SIZE_1, 4, false, false, false, 4), value_(value) {}

Size1::Size1(vector<uint8_t> value)
    : Option(OptionNumber::SIZE_1, value.size(), false, false, false, 4) {
  if (!value.empty()) {
    uint64_t concat_value = 0;
    uint8_t offset = 0;
    for (auto byte : value) {
      concat_value = concat_value | byte << offset;
      offset += 8;
    }
    value_ = concat_value;
  }
}

vector<uint8_t> Size1::getValue() { return toBytes(value_); }

string Size1::getAsString() { return to_string(value_); }

uint64_t Size1::getAsLong() { return value_; }
} // namespace CoAP