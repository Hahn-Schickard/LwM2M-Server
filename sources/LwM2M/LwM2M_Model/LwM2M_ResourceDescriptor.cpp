#include "LwM2M_ResourceDescriptor.hpp"

#include <algorithm>

using namespace std;
namespace LwM2M {

ObjectLink::ObjectLink(uint16_t object_id, uint16_t instance_id)
    : object_id_(object_id), instance_id_(instance_id) {}

RangeEnumeration::RangeEnumeration()
    : available_string_values_(), minimum_integer_value_(nullopt),
      maximum_integer_value_(nullopt), minimum_floating_value_(nullopt),
      maximum_floating_value_(nullopt) {}

RangeEnumeration::RangeEnumeration(vector<string> available_values)
    : available_string_values_(move(available_values)),
      minimum_integer_value_(nullopt), maximum_integer_value_(nullopt),
      minimum_floating_value_(nullopt), maximum_floating_value_(nullopt) {}

RangeEnumeration::RangeEnumeration(long int minimum_value,
                                   long int maximum_value)
    : available_string_values_(), minimum_integer_value_(minimum_value),
      maximum_integer_value_(maximum_value), minimum_floating_value_(nullopt),
      maximum_floating_value_(nullopt) {}

RangeEnumeration::RangeEnumeration(double minimum_value, double maximum_value)
    : available_string_values_(), minimum_integer_value_(nullopt),
      maximum_integer_value_(nullopt), minimum_floating_value_(minimum_value),
      maximum_floating_value_(maximum_value) {}

bool RangeEnumeration::inRange(string value) {
  for (string supported_value : available_string_values_) {
    if (transform(supported_value.begin(), supported_value.end(),
                  supported_value.begin(), ::toupper) ==
        transform(value.begin(), value.end(), value.begin(), ::toupper)) {
      return true;
    }
  }
  return false;
}

bool RangeEnumeration::inRange(long int value) {
  if (minimum_integer_value_ && maximum_integer_value_) {
    if (value <= maximum_integer_value_.value() &&
        value >= minimum_integer_value_.value()) {
      return true;
    }
  }
  return false;
}

bool RangeEnumeration::inRange(double value) {
  if (minimum_floating_value_ && minimum_floating_value_) {
    if (value <= minimum_floating_value_.value() &&
        value >= minimum_floating_value_.value()) {
      return true;
    }
  }
  return false;
}

ResourceDescriptor::ResourceDescriptor(const ResourceDescriptor &instance)
    : id_(instance.id_), name_(instance.name_),
      operations_(instance.operations_),
      multiple_instances_(instance.multiple_instances_),
      mandatory_(instance.mandatory_), data_type_(instance.data_type_),
      range_enumeration_(instance.range_enumeration_), units_(instance.units_),
      description_(instance.description_) {}

ResourceDescriptor::ResourceDescriptor(ResourceDescriptor &&instance)
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
                                       OperationsType operations,
                                       bool multiple_instances, bool mandatory,
                                       DataType data_type, string units,
                                       string description)
    : id_(id), name_(name), operations_(operations),
      multiple_instances_(multiple_instances), mandatory_(mandatory),
      data_type_(data_type), range_enumeration_(nullopt), units_(units),
      description_(description) {}

ResourceDescriptor::ResourceDescriptor(uint32_t id, string name,
                                       OperationsType operations,
                                       bool multiple_instances, bool mandatory,
                                       DataType data_type,
                                       RangeEnumeration range_enumeration,
                                       string units, string description)
    : id_(id), name_(name), operations_(operations),
      multiple_instances_(multiple_instances), mandatory_(mandatory),
      data_type_(data_type), range_enumeration_(move(range_enumeration)),
      units_(units), description_(description) {}
} // namespace LwM2M