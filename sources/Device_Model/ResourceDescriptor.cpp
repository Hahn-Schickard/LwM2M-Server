#include "ResourceDescriptor.hpp"

#include <algorithm>

using namespace std;
namespace LwM2M {

string toString(OperationsType operation) {
  switch (operation) {
  case OperationsType::Read: {
    return "Read";
  }
  case OperationsType::Write: {
    return "Write";
  }
  case OperationsType::Read_And_Write: {
    return "Read and Write";
  }
  case OperationsType::Execute: {
    return "Execute";
  }
  case OperationsType::No_Operation:
  default: {
    return "No Operation";
  }
  }
}

ResourceDescriptor::ResourceDescriptor(ResourceDescriptor&& instance) noexcept
    : id_(instance.id_), name_(instance.name_),
      operations_(instance.operations_),
      multiple_instances_(instance.multiple_instances_),
      mandatory_(instance.mandatory_), data_type_(instance.data_type_),
      range_enumeration_(instance.range_enumeration_), units_(instance.units_),
      description_(instance.description_) {}

ResourceDescriptor::ResourceDescriptor()
    : id_(0), name_(string()), operations_(OperationsType::No_Operation),
      multiple_instances_(false), mandatory_(false), data_type_(DataType::None),
      range_enumeration_(nullopt), units_(string()), description_(string()) {}

ResourceDescriptor::ResourceDescriptor(uint32_t id, string name,
    OperationsType operations, bool multiple_instances, bool mandatory,
    DataType data_type, string units, string description)
    : id_(id), name_(move(name)), operations_(operations),
      multiple_instances_(multiple_instances), mandatory_(mandatory),
      data_type_(data_type), range_enumeration_(nullopt), units_(move(units)),
      description_(move(description)) {}

ResourceDescriptor::ResourceDescriptor(uint32_t id, string name,
    OperationsType operations, bool multiple_instances, bool mandatory,
    DataType data_type, RangeEnumeration range_enumeration, string units,
    string description)
    : id_(id), name_(move(name)), operations_(operations),
      multiple_instances_(multiple_instances), mandatory_(mandatory),
      data_type_(data_type), range_enumeration_(range_enumeration),
      units_(move(units)), description_(move(description)) {}
} // namespace LwM2M