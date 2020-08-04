#ifndef __LWM2M_SERVER_XML_PARSER_HPP
#define __LWM2M_SERVER_XML_PARSER_HPP

#include "LwM2M_ObjectDescriptor.hpp"

#include <string>
#include <unordered_map>

namespace LwM2M {
std::unordered_map<uint32_t, std::shared_ptr<ObjectDescriptor>>
deserializeModel(const std::string &filepath);
} // namespace LwM2M

#endif //__LWM2M_SERVER_XML_PARSER_HPP