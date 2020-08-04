#include "Accept.hpp"
#include "PrimitiveConverter.hpp"

using namespace std;
using namespace utility;
namespace CoAP {
Accept::Accept() : Accept(ContentFormatType::UNRECOGNIZED) {}

Accept::Accept(ContentFormatType format_type)
    : Option(OptionNumber::ACCEPT, 2, true, false, false, 2),
      value_(format_type) {}

Accept::Accept(vector<uint8_t> value)
    : Option(OptionNumber::ACCEPT, value.size(), true, false, false, 2) {
  if (!value.empty()) {
    uint16_t concat_value = 0;
    uint8_t offset = 0;
    for (auto byte : value) {
      concat_value = concat_value | byte << offset;
      offset = 8;
    }
    value_ = toContentFormatType(concat_value);
  }
}

vector<uint8_t> Accept::getValue() { return toBytes(getAsShort()); }

string Accept::getAsString() { return toString(value_); }

uint16_t Accept::getAsShort() { return static_cast<uint16_t>(value_); }

ContentFormatType Accept::getAcceptableContentFormatType() { return value_; }
} // namespace CoAP
