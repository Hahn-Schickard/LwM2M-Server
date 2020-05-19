#include "IfNoneMatch.hpp"

#include <stdexcept>

using namespace std;
namespace CoAP {
IfNoneMatch::IfNoneMatch()
    : CoAP_Option(OptionNumber::IF_NONE_MATCH, 0, true, false, false, 0) {}

string IfNoneMatch::getValue() {
  throw domain_error("If None Match Option does not have a value to return!");
}

} // namespace CoAP
