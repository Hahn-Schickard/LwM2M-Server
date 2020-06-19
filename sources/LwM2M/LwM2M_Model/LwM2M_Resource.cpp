#include "LwM2M_Resource.hpp"

#include <algorithm>

using namespace std;
using namespace LwM2M_Model;

bool RangeEnumeration::isInRange(std::string value) {
  if (available_string_values_.has_value()) {
    for (std::string supported_value : available_string_values_.value()) {
      if (std::transform(supported_value.begin(), supported_value.end(),
                         supported_value.begin(), ::toupper) ==
          std::transform(value.begin(), value.end(), value.begin(),
                         ::toupper)) {
        return true;
      }
    }
  }
  return false;
}

bool RangeEnumeration::isInRange(long int value) {
  if (minimum_integer_value_.has_value() &&
      maximum_integer_value_.has_value()) {
    if (value <= maximum_integer_value_.value() &&
        value >= minimum_integer_value_.value()) {
      return true;
    }
  }
  return false;
}

bool RangeEnumeration::isInRange(double value) {
  if (minimum_floating_value_.has_value() &&
      minimum_floating_value_.has_value()) {
    if (value <= minimum_floating_value_.value() &&
        value >= minimum_floating_value_.value()) {
      return true;
    }
  }
  return false;
}
