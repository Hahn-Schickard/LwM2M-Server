#include "ProxyScheme.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
ProxyScheme::ProxyScheme() : ProxyScheme(string("0")) {}

ProxyScheme::ProxyScheme(string value)
    : CoAP_Option(OptionNumber::PROXY_SCHEME, value.size(), true, false, true,
                  255),
      value_(move(value)) {
  if (value.size() < 1) {
    throw out_of_range(
        "Proxy Scheme must have a value that is larger than 1 byte");
  }
}

string ProxyScheme::getValue() { return value_; }
} // namespace CoAP