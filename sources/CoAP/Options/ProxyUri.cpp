#include "ProxyUri.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
ProxyUri::ProxyUri() : ProxyUri(string("0")) {}

ProxyUri::ProxyUri(string value)
    : Option(OptionNumber::PROXY_URI, value.size(), true, false, true, 1034),
      value_(move(value)) {
  if (value.size() < 1) {
    throw out_of_range(
        "Proxy URI must have a value that is larger than 1 byte");
  }
}

string ProxyUri::getValue() { return value_; }
} // namespace CoAP