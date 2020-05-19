#include "UriHost.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
UriHost::UriHost() : UriHost(string()) {}

UriHost::UriHost(string value)
    : CoAP_Option(OptionNumber::URI_HOST, value.size(), true, false, true, 255),
      value_(move(value)) {}

string UriHost::getValue() { return value_; }
} // namespace CoAP