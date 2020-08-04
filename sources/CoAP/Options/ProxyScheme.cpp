#include "ProxyScheme.hpp"
#include "PrimitiveConverter.hpp"

#include <stdexcept>

using namespace std;
using namespace utility;
namespace CoAP {
ProxyScheme::ProxyScheme() : ProxyScheme(string("0")) {}

ProxyScheme::ProxyScheme(string value)
    : Option(OptionNumber::PROXY_SCHEME, value.size(), true, false, true, 255),
      value_(move(value)) {
  if (value.size() < 1) {
    throw out_of_range(
        "Proxy Scheme must have a value that is larger than 1 byte");
  }
}

vector<uint8_t> ProxyScheme::getValue() { return toBytes(value_); }

string ProxyScheme::getAsString() { return value_; }
} // namespace CoAP