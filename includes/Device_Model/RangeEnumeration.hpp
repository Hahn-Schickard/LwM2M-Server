#ifndef __LWM2M_RANGE_ENUMERATION_DATA_TYPE_HPP
#define __LWM2M_RANGE_ENUMERATION_DATA_TYPE_HPP

#include <optional>
#include <string>
#include <vector>

namespace LwM2M {
/**
 * @brief RangeEnumeration - represents a range of supported value types by the
 * LwM2M Resource
 *
 */
class RangeEnumeration {
  struct IntegerRange {
    long int min_;
    long int max_;

    IntegerRange(long int min, long int max) : min_(min), max_(max) {}
  };
  struct FloatRange {
    double min_;
    double max_;

    FloatRange(double min, double max) : min_(min), max_(max) {}
  };

  std::vector<std::string> available_string_values_;
  std::optional<IntegerRange> interger_range_;
  std::optional<FloatRange> float_range_;

public:
  RangeEnumeration();
  RangeEnumeration(std::vector<std::string> available_values);
  RangeEnumeration(long int minimum_value, long int maximum_value);
  RangeEnumeration(double minimum_value, double maximum_value);

  bool inRange(std::string value);
  bool inRange(long int value);
  bool inRange(double value);
};
} // namespace LwM2M

#endif //__LWM2M_RANGE_ENUMERATION_DATA_TYPE_HPP