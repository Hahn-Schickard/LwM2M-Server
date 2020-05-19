#include "UriQuery.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
UriQuery::UriQuery() : UriQuery(string()) {}

UriQuery::UriQuery(string value)
    : CoAP_Option(OptionNumber::URI_QUERY, value.size(), true, true, true, 255),
      value_(move(value)) {}

string UriQuery::getValue() { return value_; }
} // namespace CoAP