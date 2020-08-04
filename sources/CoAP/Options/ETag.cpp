#include "ETag.hpp"
#include "PrimitiveConverter.hpp"

using namespace std;
using namespace utility;
namespace CoAP {
ETag::ETag() : ETag(string()) {}

ETag::ETag(string value)
    : Option(OptionNumber::ETAG, value.size(), false, true, false, 8),
      value_(move(value)) {}

vector<uint8_t> ETag::getValue() { return toBytes(value_); }

string ETag::getAsString() { return value_; }
} // namespace CoAP
