#include "ResourceDescriptor.hpp"

#include <algorithm>

using namespace std;
namespace LwM2M {

string toString(OperationsType operation) {
  switch (operation) {
  case OperationsType::READ: {
    return "Read";
  }
  case OperationsType::WRITE: {
    return "Write";
  }
  case OperationsType::READ_AND_WRITE: {
    return "Read and Write";
  }
  case OperationsType::EXECUTE: {
    return "Execute";
  }
  case OperationsType::NO_OPERATION:
  default: {
    return "No Operation";
  }
  }
}

ResourceDescriptor::ResourceDescriptor(const ResourceDescriptor& instance)
    : id_(instance.id_), name_(instance.name_),
      operations_(instance.operations_),
      multiple_instances_(instance.multiple_instances_),
      mandatory_(instance.mandatory_), data_type_(instance.data_type_),
      range_enumeration_(instance.range_enumeration_), units_(instance.units_),
      description_(instance.description_) {}

ResourceDescriptor::ResourceDescriptor(ResourceDescriptor&& instance)
    : id_(instance.id_), name_(move(instance.name_)),
      operations_(instance.operations_),
      multiple_instances_(instance.multiple_instances_),
      mandatory_(instance.mandatory_), data_type_(instance.data_type_),
      range_enumeration_(move(instance.range_enumeration_)),
      units_(move(instance.units_)), description_(move(instance.description_)) {
}

ResourceDescriptor::ResourceDescriptor()
    : id_(0), name_(string()), operations_(OperationsType::NO_OPERATION),
      multiple_instances_(false), mandatory_(false), data_type_(DataType::NONE),
      range_enumeration_(nullopt), units_(string()), description_(string()) {}

ResourceDescriptor::ResourceDescriptor(uint32_t id, string name,
    OperationsType operations, bool multiple_instances, bool mandatory,
    DataType data_type, string units, string description)
    : id_(id), name_(name), operations_(operations),
      multiple_instances_(multiple_instances), mandatory_(mandatory),
      data_type_(data_type), range_enumeration_(nullopt), units_(units),
      description_(description) {}

ResourceDescriptor::ResourceDescriptor(uint32_t id, string name,
    OperationsType operations, bool multiple_instances, bool mandatory,
    DataType data_type, RangeEnumeration range_enumeration, string units,
    string description)
    : id_(id), name_(name), operations_(operations),
      multiple_instances_(multiple_instances), mandatory_(mandatory),
      data_type_(data_type), range_enumeration_(move(range_enumeration)),
      units_(units), description_(description) {}
} // namespace LwM2M