#include "UriPath.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
UriPath::UriPath() : UriPath(string()) {}

UriPath::UriPath(string value)
    : CoAP_Option(OptionNumber::URI_PATH, value.size(), true, true, true, 255),
      value_(move(value)) {}

string UriPath::getValue() { return value_; }
} // namespace CoAP