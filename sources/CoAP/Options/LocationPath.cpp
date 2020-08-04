#include "LocationPath.hpp"
#include "PrimitiveConverter.hpp"

using namespace std;
using namespace utility;
namespace CoAP {
LocationPath::LocationPath() : LocationPath(string()) {}

LocationPath::LocationPath(string value)
    : Option(OptionNumber::LOCATION_PATH, value.size(), false, true, false,
             255),
      value_(move(value)) {}

vector<uint8_t> LocationPath::getValue() { return toBytes(value_); }

string LocationPath::getAsString() { return value_; }
} // namespace CoAP