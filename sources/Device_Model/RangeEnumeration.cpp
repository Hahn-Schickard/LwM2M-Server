#include "RangeEnumeration.hpp"

using namespace std;
namespace LwM2M {

RangeEnumeration::RangeEnumeration(vector<string> available_values) // NOLINT
    : available_string_values_(available_values) {} // NOLINT

RangeEnumeration::RangeEnumeration(
    long int minimum_value, long int maximum_value)
    : integer_range_(IntegerRange(minimum_value, maximum_value)) {}

RangeEnumeration::RangeEnumeration(double minimum_value, double maximum_value)
    : float_range_(FloatRange(minimum_value, maximum_value)) {}

bool RangeEnumeration::inRange(const string& value) {
  bool found = false;
  for (const auto& possible_match : available_string_values_) {
    if (value == possible_match) {
      found = true;
      break;
    }
  }
  return found;
}

bool RangeEnumeration::inRange(long int value) {
  if (integer_range_.has_value()) {
    return value > integer_range_->min_ && value < integer_range_->max_;
  } else {
    return false;
  }
}

bool RangeEnumeration::inRange(double value) {
  if (float_range_.has_value()) {
    return value > float_range_->min_ && value < float_range_->max_;
  } else {
    return false;
  }
}
} // namespace LwM2M
