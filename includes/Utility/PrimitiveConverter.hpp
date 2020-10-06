#ifndef __UTILITY_PRIMITIVE_CONVERTER_HPP_
#define __UTILITY_PRIMITIVE_CONVERTER_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace utility {

std::vector<uint8_t> toBytes(bool value);

std::vector<uint8_t> toBytes(const std::string &value);

std::vector<uint8_t> toBytes(double value);
/**
 * @brief Converts a 16 bit value into an MSB first vector of bytes
 *
 * @param value
 * @return std::vector<uint8_t> first element in the vector is MSB, last is
 * LSB
 */
std::vector<uint8_t> toBytes(uint16_t value);
/**
 * @brief Converts a 32 bit value value into an MSB first vector of bytes
 *
 * @param value
 * @return std::vector<uint8_t> first element in the vector is MSB, last is LSB
 */
std::vector<uint8_t> toBytes(uint32_t value);
/**
 * @brief Converts a 64 bit value into an MSB first vector of bytes
 *
 * @param value
 * @return std::vector<uint8_t> first element in the vector is MSB, last is LSB
 */
std::vector<uint8_t> toBytes(uint64_t value);
/**
 * @brief Converts a 64 bit value into an MSB first vector of bytes
 *
 * @param value
 * @return std::vector<uint8_t> first element in the vector is MSB, last is LSB
 */
std::vector<uint8_t> toBytes(int64_t value);
} // namespace utility

#endif //__UTILITY_PRIMITIVE_CONVERTER_HPP_