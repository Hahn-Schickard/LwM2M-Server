#include "LocationPath.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
LocationPath::LocationPath() : LocationPath(string()) {}

LocationPath::LocationPath(string value)
    : CoAP_Option(OptionNumber::LOCATION_PATH, value.size(), false, true, false,
                  255),
      value_(move(value)) {}

string LocationPath::getValue() { return value_; }
} // namespace CoAP