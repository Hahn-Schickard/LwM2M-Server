#ifndef __LWM2M_CORE_LINK_DATA_TYPE_HPP
#define __LWM2M_CORE_LINK_DATA_TYPE_HPP

#include <string>
#include <unordered_map>

namespace LwM2M {
enum class CoRELinkAttribute {
  RESOURCE_TYPE,
  INTERFACE_DESCRIPTION,
  MAXIMUM_SIZE,
  CONTENT_TYPE,
  UNDEFINED
};

class CoRE_Link {
  std::string target_;
  std::unordered_map<CoRELinkAttribute, std::string> attributes_;

public:
  CoRE_Link();
  CoRE_Link(std::string utf_8_string);

  std::string getTarget();
  std::unordered_map<CoRELinkAttribute, std::string> getAttributes();
  std::string getAttributeValue(CoRELinkAttribute attribute);
  std::string toString();
};
} // namespace LwM2M

#endif //__LWM2M_CORE_LINK_DATA_TYPE_HPP