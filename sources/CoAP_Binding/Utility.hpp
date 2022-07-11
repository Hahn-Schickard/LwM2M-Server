#ifndef __LWM2M_COAP_BINDING_UTILITY_HPP
#define __LWM2M_COAP_BINDING_UTILITY_HPP

#include "CoAPS4Cpp/Message.hpp"

#include <limits>
#include <string>

namespace LwM2M {

enum class IntegerBase : uint8_t {
  AUTO = 0, /*!< determens the integer base based on prefix */
  BASE_2 = 2, /*!< Binary */
  BASE_3 = 3, /*!< Ternary */
  BASE_4 = 4, /*!< Quaternary */
  BASE_5 = 5, /*!< Quinary */
  BASE_6 = 6, /*!< Senary */
  BASE_7 = 7, /*!< Septenary */
  BASE_8 = 8, /*!< Octal */
  BASE_9 = 9, /*!< Nonary */
  BASE_10 = 10, /*!< Decimal */
  BASE_11 = 11, /*!< Undecimal */
  BASE_12 = 12, /*!< Duodecimal */
  BASE_13 = 13, /*!< Tridecimal*/
  BASE_14 = 14, /*!< Tetradecimal */
  BASE_15 = 15, /*!< Pentadecimal */
  BASE_16 = 16, /*!< Hexadecimal */
  BASE_17 = 17, /*!< Heptadecimal */
  BASE_18 = 18, /*!< Octodecimal */
  BASE_19 = 19, /*!< Enneadecimal */
  BASE_20 = 20, /*!< Vigesimal */
  BASE_21 = 21, /*!< Unvigesimal */
  BASE_22 = 22, /*!< Duovigesimal */
  BASE_23 = 23, /*!< Trivigesimal */
  BASE_24 = 24, /*!< Tetravigesimal */
  BASE_25 = 25, /*!< Pentavigesimal */
  BASE_26 = 26, /*!< Hexavigesimal */
  BASE_27 = 27, /*!< Heptavigesimal */
  BASE_28 = 28, /*!< Octovigesimal */
  BASE_29 = 29, /*!< Enneavigesimal */
  BASE_30 = 30, /*!< Trigesimal */
  BASE_31 = 31, /*!< Untrigesimal */
  BASE_32 = 32, /*!< Duotrigesimal */
  BASE_33 = 33, /*!< Tritrigesimal */
  BASE_34 = 34, /*!< Tetratrigesimal */
  BASE_35 = 35, /*!< Pentatrigesimal */
  BASE_36 = 36, /*!< Hexatrigesimal */
};
/**
 * @brief Converts a std::string to a signed integer of a given base, specified
 * by IntegerBase
 *
 * @throws std::overflow_error if a given string integer exceeds maximum
 * available signed integer limit
 * @throws std::underflow_error if a given string integer exceeds minimum
 * available signed integer limit
 * @throws std::invalid_argument if a given string integer contains non numeric
 * parts and ignore_non_numerics is not set to true
 *
 * @param integer
 * @param base
 * @param ignore_non_numerics
 * @return intmax_t
 */
inline intmax_t toSignedInteger(
    std::string integer, IntegerBase base, bool ignore_non_numerics = false) {
  const char* c_string = integer.c_str();
  char* remaining_c_string = nullptr;
  errno = 0;
  intmax_t value =
      strtoll(c_string, &remaining_c_string, static_cast<uint8_t>(base));
  if (errno == ERANGE) {
    if (value == std::numeric_limits<intmax_t>::max() ||
        value > std::numeric_limits<intmax_t>::max()) {
      std::string error_msg = integer +
          " value is too large to fit into largest availabe integer "
          "type.";
      throw std::overflow_error(error_msg);
    }
    if (value == std::numeric_limits<intmax_t>::min() ||
        value < std::numeric_limits<intmax_t>::min()) {
      std::string error_msg = integer +
          " value is too small to fit into largest availabe integer "
          "type.";
      throw std::underflow_error(error_msg);
    } else if (ignore_non_numerics && *remaining_c_string) {
      std::string error_msg = std::string(remaining_c_string) + " from " +
          integer + " is not recognizable as a integer";
      throw std::invalid_argument(error_msg);
    } else {
      std::string error_msg =
          "An unspecified range error occurred while processing string: " +
          integer + " as an signed integer";
      throw std::range_error(error_msg);
    }
  } else {
    return value;
  }
}

/**
 * @brief Converts a std::string to an unsigned integer of a given base,
 * specified by IntegerBase
 *
 * @throws std::overflow_error if a given string integer exceeds maximum
 * available unsigned integer limit
 * @throws std::invalid_argument if a given string integer contains non numeric
 * parts and ignore_non_numerics is not set to true
 *
 * @param integer
 * @param base
 * @param ignore_non_numerics
 * @return size_t
 */
inline size_t toUnsignedInteger(
    std::string integer, IntegerBase base, bool ignore_non_numerics = false) {
  const char* c_string = integer.c_str();
  char* remaining_c_string = nullptr;
  errno = 0;
  size_t value =
      strtoll(c_string, &remaining_c_string, static_cast<uint8_t>(base));
  if (errno == ERANGE) {
    if (value == std::numeric_limits<size_t>::max() ||
        value > std::numeric_limits<size_t>::max()) {
      std::string error_msg = integer +
          " value is too large to fit into largest availabe integer "
          "type.";
      throw std::overflow_error(error_msg);
    } else if (ignore_non_numerics && *remaining_c_string) {
      std::string error_msg = std::string(remaining_c_string) + " from " +
          integer + " is not recognizable as a integer";
      throw std::invalid_argument(error_msg);
    } else {
      std::string error_msg =
          "An unspecified range error occurred while processing string: " +
          integer + " as an unsigned integer";
      throw std::range_error(error_msg);
    }
  } else {
    return value;
  }
}

inline size_t generateHash(CoAP::MessagePtr message) {
  return message->getHeader()->getMessageID() << message->getToken()->size() |
      message->getToken()->hash();
}
} // namespace LwM2M

#endif //__LWM2M_COAP_BINDING_UTILITY_HPP