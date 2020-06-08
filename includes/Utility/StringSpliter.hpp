#ifndef __COAP_UTILITY_STRING_SPLITER_HPP
#define __COAP_UTILITY_STRING_SPLITER_HPP

#include <string>
#include <vector>

namespace CoAP {
namespace utility {
std::vector<std::string> split(const std::string &s, char delimiter);
}
} // namespace CoAP

#endif //__COAP_UTILITY_STRING_SPLITER_HPP