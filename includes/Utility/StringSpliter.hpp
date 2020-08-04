#ifndef __UTILITY_STRING_SPLITER_HPP
#define __UTILITY_STRING_SPLITER_HPP

#include <string>
#include <vector>

namespace utility {
/**
 * @brief Splits a given string into substrings by a given delimiter without
 * modifying the original string.
 *
 * @param s - input string
 * @param delimiter - character that marks the splitting point
 * @return std::vector<std::string>
 */
std::vector<std::string> split(const std::string &s, char delimiter);
} // namespace utility

#endif //__UTILITY_STRING_SPLITER_HPP