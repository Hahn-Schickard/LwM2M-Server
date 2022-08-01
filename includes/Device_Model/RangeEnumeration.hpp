#ifndef __LWM2M_RANGE_ENUMERATION_DATA_TYPE_HPP
#define __LWM2M_RANGE_ENUMERATION_DATA_TYPE_HPP

#include <optional>
#include <string>
#include <vector>

namespace LwM2M {
/**
 * @brief Represents a range of supported value types by the
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
  std::optional<IntegerRange> integer_range_;
  std::optional<FloatRange> float_range_;

public:
  /**
   * @brief Creates a default RangeEnumeration, used for testing only.
   *
   */
  RangeEnumeration();

  /**
   * @brief Creates a range enumeration from a given list of string literals
   *
   * @param available_values
   */
  RangeEnumeration(std::vector<std::string> available_values);

  /**
   * @brief Creates a min-max range based on given integers
   *
   * @param minimum_value
   * @param maximum_value
   */
  RangeEnumeration(long int minimum_value, long int maximum_value);

  /**
   * @brief Creates a min-max range based on given decimal values
   *
   * @param minimum_value
   * @param maximum_value
   */
  RangeEnumeration(double minimum_value, double maximum_value);

  /**
   * @brief Checks if a given string literal is within the RangeEnumeration
   * declaration
   *
   * @param value - string literal to check against
   *
   * @return true - if string literal is within the declared range
   * @return false - if string literal is outside of the declared range (also if
   * the range is integer or decimal based)
   */
  bool inRange(std::string value);

  /**
   * @brief Checks if a given integer is within the RangeEnumeration declaration
   *
   * @param value - integer value to check against
   *
   * @return true - if given integer is within the declared range
   * @return false - if given integer is outside the declared range (or if the
   * range is based on string literals)
   */
  bool inRange(long int value);

  /**
   * @brief Checks if a given decimal is within the RangeEnumeration declaration
   *
   * @param value - decimal value to check against
   *
   * @return true - if given decimal is within the declared range
   * @return false - if given decimal is outside the declared range (or if the
   * range is based on string literals)
   */
  bool inRange(double value);
};
} // namespace LwM2M

#endif //__LWM2M_RANGE_ENUMERATION_DATA_TYPE_HPP