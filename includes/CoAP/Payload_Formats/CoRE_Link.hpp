#ifndef __COAP_MESSAGE_PAYLOAD_FORMAT_CORE_LINK_HPP
#define __COAP_MESSAGE_PAYLOAD_FORMAT_CORE_LINK_HPP

#include "PayloadFormat.hpp"

#include <string>
#include <unordered_map>

namespace CoAP {

typedef enum CoRELinkAttributeEnum {
  RESOURCE_TYPE,
  INTERFACE_DESCRIPTION,
  MAXIMUM_SIZE,
  CONTENT_TYPE,
  UNDEFINED
} CoRELinkAttribute;

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

class CoRE_Links : public PayloadFormat {
  std::vector<CoRE_Link> links_;

public:
  CoRE_Links();
  CoRE_Links(std::string utf_8_string);

  std::vector<CoRE_Link> getLinks();
  std::string toString() override;
};
} // namespace CoAP

#endif //__COAP_MESSAGE_PAYLOAD_FORMAT_CORE_LINK_HPP