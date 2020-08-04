#include "UriQuery.hpp"
#include "PrimitiveConverter.hpp"

using namespace std;
using namespace utility;
namespace CoAP {
UriQuery::UriQuery() : UriQuery(string()) {}

UriQuery::UriQuery(string value)
    : Option(OptionNumber::URI_QUERY, value.size(), true, true, true, 255),
      value_(move(value)) {}

vector<uint8_t> UriQuery::getValue() { return toBytes(value_); }

string UriQuery::getAsString() { return value_; }
} // namespace CoAP