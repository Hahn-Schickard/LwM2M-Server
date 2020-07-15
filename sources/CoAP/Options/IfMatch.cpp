#include "IfMatch.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
IfMatch::IfMatch() : IfMatch(string()) {}

IfMatch::IfMatch(string value)
    : Option(OptionNumber::IF_MATCH, value.size(), true, true, false, 8),
      value_(move(value)) {}

string IfMatch::getValue() { return value_; }
} // namespace CoAP