#include "XmlParser.hpp"
#include "LoggerRepository.hpp"
#include "pugixml.hpp"

#include <filesystem>
#include <memory>
#include <type_traits>

using namespace std;
using namespace pugi;
using namespace HaSLL;
using namespace LwM2M_Model;

shared_ptr<Logger> xml_parser_logger =
    LoggerRepository::getInstance().registerLoger("XML_Parser");

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

string deserializeFile(const string &filepath) {
  xml_document document;
  string result;

  if (!document.load_file(filepath.c_str())) {
    xml_parser_logger->log(SeverityLevel::ERROR, "Cound not parse file: <>",
                           filepath);
    return result;
  }

  xml_node objects = document.child("LWM2M");

  for (xml_node object : objects.children("Object")) {
    for (xml_node property : object.children()) {
      result += deserializeNode(property);
    }
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
    throw runtime_error(error_msg);
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
    throw runtime_error(error_msg);
  }
}

template <> string getChildValue<string>(xml_node parent, string child_name) {
  if (parent.child(child_name.c_str())) {
    return string(parent.child(child_name.c_str()).child_value());
  } else {
    string error_msg = "Parent:" + string(parent.name()) +
                       " has no child named: " + child_name;
    throw runtime_error(error_msg);
  }
}

template <> bool getChildValue<bool>(xml_node parent, string child_name) {
  if (parent.child(child_name.c_str())) {
    return stod(string(parent.child(child_name.c_str()).child_value()));
  } else {
    string error_msg = "Parent:" + string(parent.name()) +
                       " has no child named: " + child_name;
    throw runtime_error(error_msg);
  }
}

OperationsType convertToOperationsType(string value) {
  if (value == "R") {
    return OperationsType::READ;
  } else if (value == "W") {
    return OperationsType::WRITE;
  } else if (value == "RW") {
    return OperationsType::READ_AND_WRITE;
  } else if (value == "E") {
    return OperationsType::EXECUTE;
  }
  return OperationsType::NO_OPERATION;
}

bool convertInstanceType(string value) {
  return value == "Multiple" ? true : false;
}

bool convertMandatoryType(string value) {
  return value == "Mandatory" ? true : false;
}

DataType converDataType(string value) {
  if (value == "String") {
    return DataType::STRING;
  } else if (value == "Integer") {
    return DataType::INTEGER;
  } else if (value == "Float") {
    return DataType::FLOAT;
  } else if (value == "Boolean") {
    return DataType::BOOLEAN;
  } else if (value == "Opaque") {
    return DataType::OPAQUE;
  } else if (value == "Time") {
    return DataType::TIME;
  } else if (value == "Objlnk") {
    return DataType::OBJECT_LINK;
  }
  return DataType::NONE;
}

optional<RangeEnumeration> getRangeEnumeration(xml_node resource_node) {
  if (!resource_node.child("RangeEnumeration").empty()) {
    // @TODO: implement RangeEnumeration convertion
  }
  return nullopt;
}

LwM2M_Resource deserializeResource(xml_node resource_node) {
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
      return LwM2M_Resource(resource_id, resource_name, resource_operations,
                            resoruce_multiple_instances, resource_mandatory,
                            resoruce_data_type, resource_range_enum.value(),
                            resource_units, resource_description);
    }
    return LwM2M_Resource(resource_id, resource_name, resource_operations,
                          resoruce_multiple_instances, resource_mandatory,
                          resoruce_data_type, resource_units,
                          resource_description);
  } catch (exception &ex) {
    string error_msg =
        "Failed to deserialize resource node: " + string(resource_node.name()) +
        " due to error: " + ex.what();
    throw runtime_error(error_msg);
  }
}

LwM2M_Object deserializeObject(xml_node object_node) {
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
    vector<LwM2M_Resource> resources;
    for (xml_node resource_node : resource_nodes.children("Item")) {
      resources.push_back(deserializeResource(resource_node));
    }
    return LwM2M_Object(object_name, object_description, object_id,
                        object_multiple_instances, object_mandatory, object_urn,
                        resources);
  } catch (exception &ex) {
    string error_msg =
        "Failed to deserialize object node: " + string(object_node.name()) +
        " due to error: " + ex.what();
    throw runtime_error(error_msg);
  }
}

vector<LwM2M_Object> deserializeModel(const string &filepath) {
  vector<LwM2M_Object> objects;
  try {
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
            objects.push_back(deserializeObject(object));
          }
        } else {
          string error_msg = "Could not open object descriptor file: " +
                             string(object_descriptor_file_path);
          throw runtime_error(error_msg);
        }
      }
    } else {
      string error_msg = "Could not open objects parent file: " + filepath;
      throw runtime_error(error_msg);
    }
  } catch (exception &ex) {
    xml_parser_logger->log(SeverityLevel::ERROR,
                           "Failled to deserialize mode: {}", ex.what());
  }
  return objects;
}