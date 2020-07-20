#include "CoRE_Link.hpp"
#include "StringSpliter.hpp"

#include <stdexcept>

using namespace std;

namespace CoAP {

CoRE_Link::CoRE_Link() {}

CoRE_Link::CoRE_Link(string utf_8_string) {

  auto link_values = utility::split(utf_8_string, ';');
  for (auto it = link_values.begin(); it != link_values.end();) {
    auto value = *it;
    size_t resource_type_index = value.find("rt=");
    if (resource_type_index != std::string::npos) {
      string resource_type = value.substr(resource_type_index + 3);
      attributes_.emplace(CoRELinkAttribute::RESOURCE_TYPE, resource_type);
      link_values.erase(it);
    }

    size_t maximum_size_type_index = value.find("sz=");
    if (maximum_size_type_index != std::string::npos) {
      string maximum_size_type = value.substr(maximum_size_type_index + 3);
      attributes_.emplace(CoRELinkAttribute::MAXIMUM_SIZE, maximum_size_type);
      link_values.erase(it);
    }

    size_t interface_description_type_index = value.find("if=");
    if (interface_description_type_index != std::string::npos) {
      string interface_description_type =
          value.substr(interface_description_type_index + 3);
      attributes_.emplace(CoRELinkAttribute::INTERFACE_DESCRIPTION,
                          interface_description_type);
      link_values.erase(it);
    }

    size_t content_type_index = value.find("ct=");
    if (content_type_index != std::string::npos) {
      string content_type = value.substr(content_type_index + 3);
      attributes_.emplace(CoRELinkAttribute::CONTENT_TYPE, content_type);
      link_values.erase(it);
    }

    size_t target_index = value.find("</");
    if (target_index != std::string::npos) {
      target_ = value.substr(target_index + 2);
      // remove extra > mark
      if (!target_.empty()) {
        target_.pop_back();
      }
      link_values.erase(it);
    }
  }
  if (!link_values.empty()) {
    string error_msg = "CoRE Link decoder could not process: ";
    for (auto link : link_values) {
      error_msg += link;
    }
    throw domain_error(error_msg);
  }
}

string CoRE_Link::getTarget() { return target_; }

unordered_map<CoRELinkAttribute, string> CoRE_Link::getAttributes() {
  return attributes_;
}

string CoRE_Link::getAttributeValue(CoRELinkAttribute attribute) {
  string result;
  auto attribute_pair = attributes_.find(attribute);
  if (attribute_pair != attributes_.end()) {
    result = attribute_pair->second;
  }
  return result;
}

string CoRE_Link::toString() {
  string result;
  result += "</" + target_ + ">";
  for (auto attribute : attributes_) {
    if (attribute.first == CoRELinkAttribute::RESOURCE_TYPE) {
      result += ";rt=" + attribute.second;
    }
    if (attribute.first == CoRELinkAttribute::MAXIMUM_SIZE) {
      result += ";sz=" + attribute.second;
    }
    if (attribute.first == CoRELinkAttribute::INTERFACE_DESCRIPTION) {
      result += ";if=" + attribute.second;
    }
  }
  return result;
}

CoRE_Links::CoRE_Links() : PayloadFormat(ContentFormatType::CORE_LINK) {}
CoRE_Links::CoRE_Links(string utf_8_string) : CoRE_Links() {
  vector<string> core_link_strings = utility::split(utf_8_string, ',');
  for (auto core_link_string : core_link_strings) {
    links_.push_back(CoRE_Link(core_link_string));
  }
}

vector<CoRE_Link> CoRE_Links::getLinks() { return links_; }

string CoRE_Links::toString() {
  string result;
  for (auto link : links_) {
    result += link.toString() + ",";
  }
  // remove extra coma
  if (!result.empty()) {
    result.pop_back();
  }
  return result;
}

size_t CoRE_Links::size() { return links_.size(); }
} // namespace CoAP
