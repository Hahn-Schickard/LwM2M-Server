#include "UriPort.hpp"
#include "PrimitiveConverter.hpp"

using namespace std;
using namespace utility;
namespace CoAP {
UriPort::UriPort() : UriPort((uint8_t)0) {}

UriPort::UriPort(uint8_t value)
    : Option(OptionNumber::URI_PORT, 1, true, false, true, 2), value_(value) {}

UriPort::UriPort(uint16_t value)
    : Option(OptionNumber::URI_PORT, 2, true, false, true, 2), value_(value) {}

UriPort::UriPort(std::vector<uint8_t> value)
    : Option(OptionNumber::URI_PORT, value.size(), true, false, true, 2) {
  if (!value.empty()) {
    uint16_t concat_value = 0;
    uint8_t offset = 0;
    for (auto byte : value) {
      concat_value = concat_value | byte << offset;
      offset += 8;
    }
    value_ = concat_value;
  }
}

vector<uint8_t> UriPort::getValue() { return toBytes(value_); }

string UriPort::getAsString() { return to_string(value_); }

uint16_t UriPort::getAsShort() { return value_; }
} // namespace CoAP