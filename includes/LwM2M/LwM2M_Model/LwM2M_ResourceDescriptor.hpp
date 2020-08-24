#ifndef __LWM2M_MODEL_RESOURCE_DESCRIPTOR_HPP
#define __LWM2M_MODEL_RESOURCE_DESCRIPTOR_HPP

#include "LwM2M_DataTypes.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace LwM2M {

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
  OPAQUE,  /*<! A sequence of binary octets. Represented as Base64 encoding of
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
 * @brief ResourceDescriptor - a representation of a single data point that is
 * modeled by the LwM2M Model
 *
 */
struct ResourceDescriptor {
  const uint32_t id_;
  const std::string name_;
  const OperationsType operations_;
  const bool multiple_instances_;
  const bool mandatory_;
  const DataType data_type_;
  const std::optional<RangeEnumeration> range_enumeration_;
  const std::string units_;
  const std::string description_;

  ResourceDescriptor();
  ResourceDescriptor(const ResourceDescriptor &instance);
  ResourceDescriptor(ResourceDescriptor &&instance);
  ResourceDescriptor(uint32_t id, std::string name, OperationsType operations,
                     bool multiple_instances, bool mandatory,
                     DataType data_type, std::string units,
                     std::string description);
  ResourceDescriptor(uint32_t id, std::string name, OperationsType operations,
                     bool multiple_instances, bool mandatory,
                     DataType data_type, RangeEnumeration range_enumeration,
                     std::string units, std::string description);
};

} // namespace LwM2M
#endif //__LWM2M_MODEL_RESOURCE_DESCRIPTOR_HPP