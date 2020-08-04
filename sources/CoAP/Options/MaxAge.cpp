#include "MaxAge.hpp"
#include "PrimitiveConverter.hpp"

#include <stdexcept>

using namespace std;
using namespace utility;
namespace CoAP {
MaxAge::MaxAge() : MaxAge((uint8_t)60) {}

MaxAge::MaxAge(uint8_t value)
    : Option(OptionNumber::MAX_AGE, 1, false, false, true, 4), value_(value) {}

MaxAge::MaxAge(uint16_t value)
    : Option(OptionNumber::MAX_AGE, 2, false, false, true, 4), value_(value) {}

MaxAge::MaxAge(uint32_t value)
    : Option(OptionNumber::MAX_AGE, 3, false, false, true, 4), value_(value) {}

MaxAge::MaxAge(uint64_t value)
    : Option(OptionNumber::MAX_AGE, 2, false, false, true, 4), value_(value) {}

MaxAge::MaxAge(vector<uint8_t> value)
    : Option(OptionNumber::MAX_AGE, value.size(), false, false, true, 4) {
  if (!value.empty()) {
    uint64_t concat_value = 0;
    uint8_t offset = 0;
    for (auto byte : value) {
      concat_value = concat_value | byte << offset;
      offset += 8;
    }
    value_ = concat_value;
  }
} // namespace CoAP

vector<uint8_t> MaxAge::getValue() { return toBytes(value_); }

string MaxAge::getAsString() { return to_string(value_); }

uint64_t MaxAge::getAsLong() { return value_; }
} // namespace CoAP