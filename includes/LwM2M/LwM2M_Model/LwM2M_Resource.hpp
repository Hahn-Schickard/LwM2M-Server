#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace LwM2M_Model {

/**
 * @enum OperationsTypeEnum
 * @brief OperationsType enumeration, specifying the supported LwM2M Resource
 * operation types
 *
 */
enum class OperationsType {
  NO_OPERATION,
  READ,
  WRITE,
  READ_AND_WRITE,
  EXECUTE
};

/**
 * @enum DataTypeEnum
 * @brief DataType enumeration, specifying the supported LwM2M Resource data
 * types
 *
 */
enum class DataType {
  NONE,    /*!< No specific data type is applicable to this resource (Only for
              Executable or Malformated Resources) */
  STRING,  /*!< UTF-8 String, represented as UTF-8 string for both Text and TLV
              formats */
  INTEGER, /*!< 8/16/32/64 bit signed integer value, specific value type
              indicated by the length field which is resolved internally.
              Represented as an ASCII character for Text Format and as a Binary
              signed integer in network byte order. Sent
              as big endian over the network*/
  FLOAT,   /*!< 32/64 bit floating point value, specific value type indicated by
              the length field   which is resolved internally. Represented as an
              ASCII string in Text and as a binary floating point per definiotion
              of IEEE 754-2008 in TLV format. Sent as big endian over the network
            */
  BOOLEAN, /*<! An 8 bit unsigned integer value containign 0 for FALSE or 1 for
              TRUE, Represented as 0 or 1 ASCII charater in Text and as an 8 bit
              unsigned integer with value of 0 or 1 in TLV format */
  OPAQUE,  /*<! A sequence of binary octets. Represented as Bas64 encoding of
              binary data as defined per RFC4648 in ASCII format and as a
              sequence of binary data limited by the length field in TLV format.
            */
  TIME,    /* A signed integer containing Unix time. Represented as and ASCII
              charater in Text and as a signeed Integer in TLV format. */
  OBJECT_LINK /* Two concatenated 16 bit unsigned intergers in Network Byte
                 Order, Most significant Half-word (left most 16 bits) indicate
                 the Object ID and the Least Significant Half-word (right most
                 16 bits) indicate the Object Instance ID. Represented as a
                 UTF-8 string containign 2 16-bit ASCII integers seperated by
                 ':' in Text format or as two 16 bit unsigned integers in TLV
                 Format*/
};

/**
 * @brief ObjectLink - indicates a link to another object
 *
 */
typedef struct ObjectLink {
  const uint16_t object_id_;
  const uint16_t instance_id_;

  ObjectLink(uint16_t object_id, uint16_t instance_id)
      : object_id_(object_id), instance_id_(instance_id) {}
} ObjectLink;

/**
 * @brief RangeEnumeration - represents a range of supported value types by the
 * LwM2M Resource
 *
 */
struct RangeEnumeration {
  const std::optional<std::vector<std::string>> available_string_values_;
  const std::optional<long int> minimum_integer_value_;
  const std::optional<long int> maximum_integer_value_;
  const std::optional<double> minimum_floating_value_;
  const std::optional<double> maximum_floating_value_;

public:
  RangeEnumeration()
      : available_string_values_(std::nullopt),
        minimum_integer_value_(std::nullopt),
        maximum_integer_value_(std::nullopt),
        minimum_floating_value_(std::nullopt),
        maximum_floating_value_(std::nullopt) {}

  RangeEnumeration(std::vector<std::string> available_values)
      : available_string_values_(std::move(available_values)),
        minimum_integer_value_(std::nullopt),
        maximum_integer_value_(std::nullopt),
        minimum_floating_value_(std::nullopt),
        maximum_floating_value_(std::nullopt) {}

  RangeEnumeration(long int minimum_value, long int maximum_value)
      : available_string_values_(std::nullopt),
        minimum_integer_value_(minimum_value),
        maximum_integer_value_(maximum_value),
        minimum_floating_value_(std::nullopt),
        maximum_floating_value_(std::nullopt) {}

  RangeEnumeration(double minimum_value, double maximum_value)
      : available_string_values_(std::nullopt),
        minimum_integer_value_(std::nullopt),
        maximum_integer_value_(std::nullopt),
        minimum_floating_value_(minimum_value),
        maximum_floating_value_(maximum_value) {}

  bool isInRange(std::string value);
  bool isInRange(long int value);
  bool isInRange(double value);
};

/**
 * @brief LwM2M_Resource - a representation of a single data point that is
 * modeled by the LwM2M Model
 *
 */
struct LwM2M_Resource {
  const uint32_t id_;
  const std::string name_;
  const OperationsType operations_;
  const bool multiple_instances_;
  const bool mandatory_;
  const DataType data_type_;
  const std::optional<RangeEnumeration> range_enumeration_;
  const std::string units_;
  const std::string description_;

  LwM2M_Resource(const LwM2M_Model::LwM2M_Resource &instance)
      : id_(instance.id_), name_(instance.name_),
        operations_(instance.operations_),
        multiple_instances_(instance.multiple_instances_),
        mandatory_(instance.mandatory_), data_type_(instance.data_type_),
        range_enumeration_(instance.range_enumeration_),
        units_(instance.units_), description_(instance.description_) {}

  LwM2M_Resource(LwM2M_Resource &&instance)
      : id_(instance.id_), name_(std::move(instance.name_)),
        operations_(instance.operations_),
        multiple_instances_(instance.multiple_instances_),
        mandatory_(instance.mandatory_), data_type_(instance.data_type_),
        range_enumeration_(std::move(instance.range_enumeration_)),
        units_(std::move(instance.units_)),
        description_(std::move(instance.description_)) {}

  LwM2M_Resource()
      : id_(0), name_(std::string()), operations_(OperationsType::NO_OPERATION),
        multiple_instances_(false), mandatory_(false),
        data_type_(DataType::NONE), range_enumeration_(std::nullopt),
        units_(std::string()), description_(std::string()) {}

  LwM2M_Resource(uint32_t id, std::string name, OperationsType operations,
                 bool multiple_instances, bool mandatory, DataType data_type,
                 std::string units, std::string description)
      : id_(id), name_(name), operations_(operations),
        multiple_instances_(multiple_instances), mandatory_(mandatory),
        data_type_(data_type), range_enumeration_(std::nullopt), units_(units),
        description_(description) {}

  LwM2M_Resource(uint32_t id, std::string name, OperationsType operations,
                 bool multiple_instances, bool mandatory, DataType data_type,
                 RangeEnumeration range_enumeration, std::string units,
                 std::string description)
      : id_(id), name_(name), operations_(operations),
        multiple_instances_(multiple_instances), mandatory_(mandatory),
        data_type_(data_type), range_enumeration_(std::move(range_enumeration)),
        units_(units), description_(description) {}
};

} // namespace LwM2M_Model
#endif //__LWM2M_MODEL_RESOURCE_HPP