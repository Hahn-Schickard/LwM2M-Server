#ifndef __LWM2M_DATA_FORMAT_TYPE_HPP
#define __LWM2M_DATA_FORMAT_TYPE_HPP

#include "ObjectLink.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <stdexcept>

namespace LwM2M {

/**
 * @enum MediaType
 * @brief Enumeration of supported IANA Media Types
 *
 */
enum class MediaType : uint16_t {
  PLAIN_TEXT = 0,   /*!< text/plain */
  CoRE_Link = 40,   /*!< application//link-format */
  OPAQUE = 42,      /*!< application/octet-stream */
  CBOR = 60,        /*!< application/cbor */
  SenML_JSON = 110, /*!< application/senml+json */
  SenML_CBOR = 112, /*!< application/senml+cbor */
  TLV = 11542,      /*!< application/vnd.oma.lwm2m+tlv */
  JSON = 11543,     /*!< application/vnd.oma.lwm2m+json */
  UNRECOGNIZED
};

/**
 * @enum DataType
 * @brief DataType enumeration, specifying the supported LwM2M Resource data
 * types
 *
 */
enum class DataType : uint8_t {
  NONE,   /*!< No specific data type is applicable to this resource (Only for
             Executable or Malformated Resources) */
  STRING, /*!< UTF-8 String, represented as UTF-8 string for both Text and TLV
             formats */
  SIGNED_INTEGER,   /*!< 64 bit signed integer value, specific value type
                indicated by the length field which is resolved internally.
                Represented as an ASCII character for Text Format and as a Binary
                signed integer in network byte order. Sent
                as big endian over the network*/
  UNSIGNED_INTEGER, /*< 64 bit unsigned integer value*/
  FLOAT,   /*!< 64 bit floating point value, specific value type indicated by
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

std::string toString(DataType type);

struct WrongDataType : public std::logic_error {
  WrongDataType(DataType expected, DataType actual)
      : std::logic_error("Wrong actual data type!\nExpected data type: " +
                         toString(expected) +
                         "\nActual data type: " + toString(actual)) {}
};

/**
 * @brief DataFormat - data and media type abstraction for all of the media
 * formats defined in LwM2M::MediaType
 *
 */
class DataFormat {
  MediaType media_type_;
  DataType data_type_;

public:
  DataFormat(MediaType format, DataType type)
      : media_type_(format), data_type_(type) {}
  virtual ~DataFormat() = default;

  MediaType getMediaType() { return media_type_; }

  DataType getDataType() { return data_type_; }

  virtual bool asBoolean() {
    if (data_type_ != DataType::BOOLEAN) {
      throw WrongDataType(DataType::BOOLEAN, data_type_);
    }
  }

  virtual int64_t asSignedInteger() {
    if (data_type_ != DataType::SIGNED_INTEGER) {
      throw WrongDataType(DataType::SIGNED_INTEGER, data_type_);
    }
  }

  virtual uint64_t asUnsignedInteger() {
    if (data_type_ != DataType::UNSIGNED_INTEGER) {
      throw WrongDataType(DataType::UNSIGNED_INTEGER, data_type_);
    }
  }

  virtual double asDouble() {
    if (data_type_ != DataType::FLOAT) {
      throw WrongDataType(DataType::FLOAT, data_type_);
    }
  }

  virtual std::string asString() {
    if (data_type_ != DataType::STRING) {
      throw WrongDataType(DataType::STRING, data_type_);
    }
  }

  virtual ObjectLink asObjectLink() {
    if (data_type_ != DataType::OBJECT_LINK) {
      throw WrongDataType(DataType::OBJECT_LINK, data_type_);
    }
  }

  virtual std::vector<uint8_t> asOpaque() {
    if (data_type_ != DataType::OPAQUE) {
      throw WrongDataType(DataType::OPAQUE, data_type_);
    }
  }
};

using DataFormatPtr = std::shared_ptr<DataFormat>;
} // namespace LwM2M

#endif //__LWM2M_DATA_FORMAT_TYPE_HPP