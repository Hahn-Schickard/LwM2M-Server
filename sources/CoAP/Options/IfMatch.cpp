#include "IfMatch.hpp"
#include "PrimitiveConverter.hpp"

using namespace std;
using namespace utility;
namespace CoAP {
IfMatch::IfMatch() : IfMatch(string()) {}

IfMatch::IfMatch(string value)
    : Option(OptionNumber::IF_MATCH, value.size(), true, true, false, 8),
      value_(move(value)) {}

vector<uint8_t> IfMatch::getValue() { return toBytes(value_); }

string IfMatch::getAsString() { return value_; }
} // namespace CoAP