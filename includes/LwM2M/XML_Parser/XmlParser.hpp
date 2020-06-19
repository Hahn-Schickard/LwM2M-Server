#ifndef __LWM2M_SERVER_XML_PARSER_HPP
#define __LWM2M_SERVER_XML_PARSER_HPP

#include "LwM2M_Object.hpp"

#include <string>
#include <unordered_map>

namespace LwM2M_Model {
std::string deserializeFile(const std::string &filepath);

/**
 * @brief Deserializes LwM2M XML Descriptors into the Object Model
 *
 * @param filepath - XML file containing a list of LwM2M Object descriptor
 * locations
 * @return std::vector<LwM2M_Model::LwM2M_Object>
 */
std::unordered_map<uint32_t, LwM2M_Model::LwM2M_Object>
deserializeModel(const std::string &filepath);
} // namespace LwM2M_Model

#endif //__LWM2M_SERVER_XML_PARSER_HPP