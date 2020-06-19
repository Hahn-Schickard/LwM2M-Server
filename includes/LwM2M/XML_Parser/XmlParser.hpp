#ifndef __LWM2M_SERVER_XML_PARSER_HPP
#define __LWM2M_SERVER_XML_PARSER_HPP

#include "LwM2M_Object.hpp"

#include <string>

namespace LwM2M_Model {
std::string deserializeFile(const std::string &filepath);
std::vector<LwM2M_Model::LwM2M_Object>
deserializeModel(const std::string &filepath);
} // namespace LwM2M_Model

#endif //__LWM2M_SERVER_XML_PARSER_HPP