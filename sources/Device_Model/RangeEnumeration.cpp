#include "RangeEnumeration.hpp"

using namespace std;
namespace LwM2M {

RangeEnumeration::RangeEnumeration() {}

RangeEnumeration::RangeEnumeration(vector<string> available_values)
    : available_string_values_(available_values) {}

RangeEnumeration::RangeEnumeration(
    long int minimum_value, long int maximum_value)
    : interger_range_(IntegerRange(minimum_value, maximum_value)) {}

RangeEnumeration::RangeEnumeration(double minimum_value, double maximum_value)
    : float_range_(FloatRange(minimum_value, maximum_value)) {}

bool RangeEnumeration::inRange(string value) {
  bool found = false;
  for (auto possible_match : available_string_values_) {
    if (value == possible_match) {
      found = true;
      break;
    }
  }
  return found;
}

bool RangeEnumeration::inRange(long int value) {
  if (interger_range_.has_value()) {
    if (value > interger_range_->min_ && value < interger_range_->max_) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool RangeEnumeration::inRange(double value) {
  if (float_range_.has_value()) {
    if (value > float_range_->min_ && value < float_range_->max_) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}
} // namespace LwM2M
