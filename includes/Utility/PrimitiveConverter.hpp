#ifndef __UTILITY_PRIMITIVE_CONVERTER_HPP_
#define __UTILITY_PRIMITIVE_CONVERTER_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace utility {

std::vector<uint8_t> toBytes(const std::string &value);

std::vector<uint8_t> toBytes(double value);

std::vector<uint8_t> toBytes(bool value);

std::vector<uint8_t> toBytes(uint16_t value);

std::vector<uint8_t> toBytes(uint32_t value);

std::vector<uint8_t> toBytes(uint64_t value);

std::vector<uint8_t> toBytes(int64_t value);
} // namespace utility

#endif //__UTILITY_PRIMITIVE_CONVERTER_HPP_