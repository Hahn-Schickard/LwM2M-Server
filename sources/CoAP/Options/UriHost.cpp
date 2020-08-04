#include "UriHost.hpp"
#include "PrimitiveConverter.hpp"

using namespace std;
using namespace utility;
namespace CoAP {
UriHost::UriHost() : UriHost(string()) {}

UriHost::UriHost(string value)
    : Option(OptionNumber::URI_HOST, value.size(), true, false, true, 255),
      value_(move(value)) {}

vector<uint8_t> UriHost::getValue() { return toBytes(value_); }

string UriHost::getAsString() { return value_; }
} // namespace CoAP