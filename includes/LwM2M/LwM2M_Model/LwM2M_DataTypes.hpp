#ifndef __LWM2M_NON_TRIVIAL_DATA_TYPES_HPP
#define __LWM2M_NON_TRIVIAL_DATA_TYPES_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace LwM2M {
/**
 * @brief ObjectLink - indicates a link to another object
 *
 */
struct ObjectLink {
  const uint16_t object_id_;
  const uint16_t instance_id_;

  ObjectLink(uint16_t object_id, uint16_t instance_id);
};

/**
 * @brief Opaque a sequenc of binary data
 *
 */
using Opaque = std::vector<uint8_t>;

/**
 * @brief RangeEnumeration - represents a range of supported value types by the
 * LwM2M Resource
 *
 */
class RangeEnumeration {
  const std::vector<std::string> available_string_values_;
  const std::optional<long int> minimum_integer_value_;
  const std::optional<long int> maximum_integer_value_;
  const std::optional<double> minimum_floating_value_;
  const std::optional<double> maximum_floating_value_;

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

#endif //__LWM2M_NON_TRIVIAL_DATA_TYPES_HPP