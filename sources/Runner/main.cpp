#include "CoAP_Server.hpp"
#include "LoggerRepository.hpp"
#include "LwM2M_Object.hpp"
#include "UDPClient.hpp"
#include "XmlParser.hpp"

#include <iostream>
#include <thread>
#include <vector>

using namespace HaSLL;
using namespace std;

string convertDatatype(LwM2M_Model::DataType data_type) {
  switch (data_type) {
  case LwM2M_Model::DataType::BOOLEAN: {
    return "Boolean";
  }
  case LwM2M_Model::DataType::INTEGER: {
    return "Integer";
  }
  case LwM2M_Model::DataType::FLOAT: {
    return "Float";
  }
  case LwM2M_Model::DataType::STRING: {
    return "String";
  }
  case LwM2M_Model::DataType::OPAQUE: {
    return "Opaque";
  }
  case LwM2M_Model::DataType::TIME: {
    return "Time";
  }
  case LwM2M_Model::DataType::OBJECT_LINK: {
    return "Object Link";
  }
  case LwM2M_Model::DataType::NONE:
  default:
    return "None";
  }
}

string convertOperations(LwM2M_Model::OperationsType operations) {
  switch (operations) {
  case LwM2M_Model::OperationsType::READ: {
    return "Read";
  }
  case LwM2M_Model::OperationsType::WRITE: {
    return "Write";
  }
  case LwM2M_Model::OperationsType::READ_AND_WRITE: {
    return "Read and Write";
  }
  case LwM2M_Model::OperationsType::EXECUTE: {
    return "Execute";
  }
  case LwM2M_Model::OperationsType::NO_OPERATION:
  default:
    return "None";
  }
}

void printRangeEnum(const LwM2M_Model::RangeEnumeration *range_enum,
                    size_t offset) {
  if (range_enum->available_string_values_.has_value()) {
    cout << string(offset, ' ') << "Available values:" << endl;
    for (auto value : range_enum->available_string_values_.value()) {
      cout << string(offset + 3, ' ') << value << endl;
    }
  } else if (range_enum->minimum_integer_value_.has_value() &&
             range_enum->maximum_integer_value_.has_value()) {
    cout << string(offset, ' ') << "Values in range: ["
         << range_enum->minimum_integer_value_.value() << ";"
         << range_enum->maximum_integer_value_.value() << "]" << endl;
  } else if (range_enum->minimum_floating_value_.has_value() &&
             range_enum->maximum_floating_value_.has_value()) {
    cout << string(offset, ' ') << "Values in range: ["
         << range_enum->minimum_floating_value_.value() << ";"
         << range_enum->maximum_floating_value_.value() << "]" << endl;
  }
}

void printResource(const LwM2M_Model::LwM2M_Resource *resource, size_t offset) {
  cout << string(offset, '=') << "====== RESOURCE " << resource->id_
       << " ======" << endl;
  cout << string(offset, ' ') << "Name: " << resource->name_ << endl;
  cout << string(offset, ' ') << "Description: " << resource->description_
       << endl;
  cout << string(offset, ' ') << "Units: " << resource->units_ << endl;
  cout << string(offset, ' ') << "Multiple instances: "
       << (resource->multiple_instances_ ? "True" : "False") << endl;
  cout << string(offset, ' ')
       << "Mandatory: " << (resource->mandatory_ ? "True" : "False") << endl;
  cout << string(offset, ' ')
       << "DataType: " << convertDatatype(resource->data_type_) << endl;
  cout << string(offset, ' ')
       << "Operations: " << convertOperations(resource->operations_) << endl;
  if (resource->range_enumeration_.has_value()) {
    printRangeEnum(&resource->range_enumeration_.value(), offset);
  }
}

void printObject(const LwM2M_Model::LwM2M_Object *object, size_t offset) {
  cout << string(offset, '=') << "====== OBJECT " << object->id_
       << " ======" << endl;
  cout << string(offset, ' ') << "Name: " << object->name_ << endl;
  cout << string(offset, ' ') << "URN: " << object->urn_ << endl;
  cout << string(offset, ' ') << "Description: " << object->description_
       << endl;
  cout << string(offset, ' ') << "Multiple instances: "
       << (object->multiple_instances_ ? "True" : "False") << endl;
  cout << string(offset, ' ')
       << "Mandatory: " << (object->mandatory_ ? "True" : "False") << endl;
  for (auto resource : object->resources_) {
    printResource(&resource, offset + 3);
  }
}

void printModel(vector<LwM2M_Model::LwM2M_Object> lwm2m_model) {
  for (auto object : lwm2m_model) {
    printObject(&object, 0);
  }
}

int main() {

  auto logger = LoggerRepository::getInstance().registerLoger("Example_Runner");
  LoggerRepository::getInstance().configure(SeverityLevel::TRACE);

  thread server_thread;

  try {
    LwM2M_Server::CoAP_Server server(false, 13251, 1);
    server_thread = thread([&]() { server.run(); });
    this_thread::sleep_for(chrono::seconds(1));
    string buffer = LwM2M_Client::receive(false, "127.0.0.1", 13251);
    logger->log(SeverityLevel::INFO, "Current time is: {}", buffer);
    cout << "Current time is: " << buffer << endl;
    server.stop();
    server_thread.join();
    printModel(deserializeModel("example_model/ipsoConfig.xml"));
  } catch (exception &e) {
    logger->log(SeverityLevel::ERROR, "Received an exception: {}", e.what());
    cerr << e.what();
  }

  exit(EXIT_SUCCESS);
}