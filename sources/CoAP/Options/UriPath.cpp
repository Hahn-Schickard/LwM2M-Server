#include "UriPath.hpp"
#include "PrimitiveConverter.hpp"

using namespace std;
using namespace utility;
namespace CoAP {
UriPath::UriPath() : UriPath(string()) {}

UriPath::UriPath(string value)
    : Option(OptionNumber::URI_PATH, value.size(), true, true, true, 255),
      value_(move(value)) {}

vector<uint8_t> UriPath::getValue() { return toBytes(value_); }

string UriPath::getAsString() { return value_; }
} // namespace CoAP