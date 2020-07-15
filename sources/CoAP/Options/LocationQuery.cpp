#include "LocationQuery.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
LocationQuery::LocationQuery() : LocationQuery(string()) {}

LocationQuery::LocationQuery(string value)
    : Option(OptionNumber::LOCATION_QUERY, value.size(), false, true, false,
             255),
      value_(move(value)) {}

string LocationQuery::getValue() { return value_; }
} // namespace CoAP
