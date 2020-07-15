#include "ETag.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
ETag::ETag() : ETag(string()) {}

ETag::ETag(string value)
    : Option(OptionNumber::ETAG, value.size(), false, true, false, 8),
      value_(move(value)) {}

string ETag::getValue() { return value_; }
} // namespace CoAP
