#include "LocationQuery.hpp"
#include "PrimitiveConverter.hpp"

using namespace std;
using namespace utility;
namespace CoAP {
LocationQuery::LocationQuery() : LocationQuery(string()) {}

LocationQuery::LocationQuery(string value)
    : Option(OptionNumber::LOCATION_QUERY, value.size(), false, true, false,
             255),
      value_(move(value)) {}

vector<uint8_t> LocationQuery::getValue() { return toBytes(value_); }

string LocationQuery::getAsString() { return value_; }
} // namespace CoAP
