#include "XmlParser.hpp"
#include "pugixml.hpp"

#include <filesystem>
#include <memory>
#include <stdexcept>

using namespace std;
using namespace pugi;

namespace LwM2M {
string deserializeNode(xml_node node) {
  string result;
  if (node.first_child() != node.last_child()) {
    for (xml_node subnode : node.children()) {
      result += deserializeNode(subnode);
    }
  } else {
    result += string(node.name()) + "=" + string(node.child_value()) + "\n\r";
  }
  return result;
}

template <typename T> T getChildValue(xml_node parent, string child_name) {
  throw runtime_error("Unsupported data type requested");
}

template <>
uint32_t getChildValue<uint32_t>(xml_node parent, string child_name) {
  if (parent.child(child_name.c_str())) {
    return (uint32_t)stol(
        string(parent.child(child_name.c_str()).child_value()));
  } else {
    string error_msg = "Parent:" + string(parent.name()) +
                       " has no child named: " + child_name;
    throw runtime_error(move(error_msg));
  }
}

template <> double getChildValue<double>(xml_node parent, string child_name) {
  if (parent.child(child_name.c_str())) {
    return string(parent.child(child_name.c_str()).child_value()) == "true"
               ? true
               : false;
  } else {
    string error_msg = "Parent:" + string(parent.name()) +
                       " has no child named: " + child_name;
    throw runtime_error(move(error_msg));
  }
}

template <> string getChildValue<string>(xml_node parent, string child_name) {
  if (parent.child(child_name.c_str())) {
    return string(parent.child(child_name.c_str()).child_value());
  } else {
    string error_msg = "Parent:" + string(parent.name()) +
                       " has no child named: " + child_name;
    throw runtime_error(move(error_msg));
  }
}

template <> bool getChildValue<bool>(xml_node parent, string child_name) {
  if (parent.child(child_name.c_str())) {
    return stod(string(parent.child(child_name.c_str()).child_value()));
  } else {
    string error_msg = "Parent:" + string(parent.name()) +
                       " has no child named: " + child_name;
    throw runtime_error(move(error_msg));
  }
}

OperationsType convertToOperationsType(string value) {
  OperationsType result;
  if (value == "R") {
    result = OperationsType::READ;
  } else if (value == "W") {
    result = OperationsType::WRITE;
  } else if (value == "RW") {
    result = OperationsType::READ_AND_WRITE;
  } else if (value == "E") {
    result = OperationsType::EXECUTE;
  } else {
    result = OperationsType::NO_OPERATION;
  }
  return result;
}

bool convertInstanceType(string value) {
  return value == "Multiple" ? true : false;
}

bool convertMandatoryType(string value) {
  return value == "Mandatory" ? true : false;
}

DataType converDataType(string value) {
  DataType result;
  if (value == "String") {
    result = DataType::STRING;
  } else if (value == "Integer") {
    result = DataType::SIGNED_INTEGER;
  } else if (value == "Float") {
    result = DataType::FLOAT;
  } else if (value == "Boolean") {
    result = DataType::BOOLEAN;
  } else if (value == "Opaque") {
    result = DataType::OPAQUE;
  } else if (value == "Time") {
    result = DataType::TIME;
  } else if (value == "Time") {
    result = DataType::TIME;
  } else if (value == "Unsigned Integer") {
    result = DataType::UNSIGNED_INTEGER;
  } else if (value == "Corelnk") {
    result = DataType::CORE_LINK;
  } else {
    result = DataType::NONE;
  }
  return result;
}

optional<RangeEnumeration> getRangeEnumeration(xml_node resource_node) {
  if (!resource_node.child("RangeEnumeration").empty()) {
    // @TODO: implement RangeEnumeration convertion
  }
  return nullopt;
}

ResourceDescriptorPtr deserializeResource(xml_node resource_node) {
  try {
    uint32_t resource_id;
    if (resource_node.attribute("ID").hash_value()) {
      resource_id = resource_node.attribute("ID").as_ullong();
    } else {
      throw runtime_error("Resource does not hane an Item ID.");
    }
    auto resource_name = getChildValue<string>(resource_node, "Name");
    auto resource_operations = convertToOperationsType(
        getChildValue<string>(resource_node, "Operations"));
    auto resoruce_multiple_instances = convertInstanceType(
        getChildValue<string>(resource_node, "MultipleInstances"));
    auto resource_mandatory =
        convertMandatoryType(getChildValue<string>(resource_node, "Mandatory"));
    auto resoruce_data_type =
        converDataType(getChildValue<string>(resource_node, "Type"));
    auto resource_range_enum = getRangeEnumeration(resource_node);
    auto resource_units = getChildValue<string>(resource_node, "Units");
    auto resource_description =
        getChildValue<string>(resource_node, "Description");

    if (resource_range_enum.has_value()) {
      return make_shared<ResourceDescriptor>(
          resource_id, resource_name, resource_operations,
          resoruce_multiple_instances, resource_mandatory, resoruce_data_type,
          resource_range_enum.value(), resource_units, resource_description);
    } else {
      return make_shared<ResourceDescriptor>(
          resource_id, resource_name, resource_operations,
          resoruce_multiple_instances, resource_mandatory, resoruce_data_type,
          resource_units, resource_description);
    }
  } catch (exception &ex) {
    string error_msg =
        "Failed to deserialize resource node: " + string(resource_node.name()) +
        " due to error: " + ex.what();
    throw runtime_error(move(error_msg));
  }
}

ObjectDescriptorPtr deserializeObject(xml_node object_node) {
  try {
    auto object_name = getChildValue<string>(object_node, "Name");
    auto object_description =
        getChildValue<string>(object_node, "Description1") +
        getChildValue<string>(object_node, "Description2");
    auto object_id = getChildValue<uint32_t>(object_node, "ObjectID");
    auto object_urn = getChildValue<string>(object_node, "ObjectURN");
    auto object_multiple_instances = convertInstanceType(
        getChildValue<string>(object_node, "MultipleInstances"));
    auto object_mandatory =
        convertMandatoryType(getChildValue<string>(object_node, "Mandatory"));
    xml_node resource_nodes = object_node.child("Resources");
    unordered_map<uint32_t, shared_ptr<ResourceDescriptor>> resources;
    for (xml_node resource_node : resource_nodes.children("Item")) {
      auto resource = deserializeResource(resource_node);
      resources.emplace(resource->id_, move(resource));
    }
    return make_shared<ObjectDescriptor>(
        object_name, object_description, object_id, object_multiple_instances,
        object_mandatory, object_urn, resources);
  } catch (exception &ex) {
    string error_msg =
        "Failed to deserialize object node: " + string(object_node.name()) +
        " due to error: " + ex.what();
    throw runtime_error(move(error_msg));
  }
}

unordered_map<uint32_t, shared_ptr<ObjectDescriptor>>
deserializeModel(const string &filepath) {
  unordered_map<uint32_t, shared_ptr<ObjectDescriptor>> objects;
  xml_document objects_document;
  filesystem::path root_path = filesystem::path(filepath).remove_filename();
  if (objects_document.load_file(filepath.c_str())) {
    xml_node obpejct_descriptor_path = objects_document.child("IPSOModel");
    for (xml_node object_file_path :
         obpejct_descriptor_path.children("IPSOPath")) {
      filesystem::path object_descriptor_file_path = root_path;
      object_descriptor_file_path +=
          object_file_path.attribute("File").as_string();
      xml_document object_descripotr;
      if (object_descripotr.load_file(object_descriptor_file_path.c_str())) {
        for (auto object :
             object_descripotr.child("LWM2M").children("Object")) {
          auto ObjectDescriptor = deserializeObject(object);
          objects.emplace(ObjectDescriptor->id_, ObjectDescriptor);
        }
      } else {
        string error_msg = "Could not open object descriptor file: " +
                           string(object_descriptor_file_path);
        throw runtime_error(move(error_msg));
      }
    }
  } else {
    string error_msg = "Could not open objects parent file: " + filepath;
    throw runtime_error(move(error_msg));
  }
  return objects;
}
} // namespace LwM2M