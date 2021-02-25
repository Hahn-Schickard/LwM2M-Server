#ifndef __LWM2M_DATA_FORMAT_TYPE_HPP
#define __LWM2M_DATA_FORMAT_TYPE_HPP

#include "ElementID.hpp"
#include "ObjectLink.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace LwM2M {

/**
 * @enum MediaType
 * @brief Enumeration of supported IANA Media Types
 *
 */
enum class MediaType : uint16_t {
  PLAIN_TEXT = 0,   /*!< text/plain */
  CORE_LINK = 40,   /*!< application//link-format */
  OPAQUE = 42,      /*!< application/octet-stream */
  CBOR = 60,        /*!< application/cbor */
  SENML_JSON = 110, /*!< application/senml+json */
  SENML_CBOR = 112, /*!< application/senml+cbor */
  TLV = 11542,      /*!< application/vnd.oma.lwm2m+tlv */
  JSON = 11543,     /*!< application/vnd.oma.lwm2m+json */
  NOT_SPECIFIED
};

std::string toString(MediaType type);

/**
 * @enum DataType
 * @brief DataType enumeration, specifying the supported LwM2M Resource data
 * types
 *
 */
enum class DataType : uint8_t {
  NONE,   /*!< No specific data type is applicable to this resource (Only for
             Executable or Malformated Resources) */
  STRING, /*!< UTF-8 String */
  SIGNED_INTEGER,   /*!< 64 bit signed integer value, represented as int64_t */
  UNSIGNED_INTEGER, /*!< 64 bit unsigned integer value*/
  FLOAT,   /*!< 64 bit floating point value represented as double precision
              floating point (double) */
  BOOLEAN, /*!< 8 bit unsigned integer value containing 0 for FALSE or 1 for
              TRUE */
  OPAQUE,  /*!< A sequence of binary octets represented as a vector of uint8_t
              values */
  TIME, /*!< A signed integer containing Unix time represented as uint64_t value
         */
  OBJECT_LINK, /*!< Refers to an Instance of a given Object, represented as
                  LwM2M::ObjectLink class */
  CORE_LINK /*!< Used to refer to Resources and their attributs as specified in
               RFC6690 */
};

std::string toString(DataType type);

struct UnsuportedDataType : public std::logic_error {
  UnsuportedDataType();
};

using DataVariant = std::variant<bool, int64_t, uint64_t, double, std::string,
                                 ObjectLink, std::vector<uint8_t>>;

/**
 * @brief DataFormat - LwM2M data type cotainer
 *
 */
struct DataFormat {
  const std::vector<uint8_t> data_;

  DataFormat(std::vector<uint8_t> bytes);
  DataFormat(DataVariant data);

  /**
   * @brief Helper method to access underling DataVariant without defining a
   * visitor.
   *
   * @throw LwM2M::UnsuportedDataType - on accesing non existant data type
   * @throw LwM2M::WrongDataType - on accessing incorrect data type
   * @throw std::bad_variant_access - on malformated DataVariant value
   *
   * @tparam T - requested data type
   * @return T - contained data value as type T
   */
  template <typename T> T get() const { throw UnsuportedDataType(); }
  size_t size() const;

  friend bool operator==(const DataFormat &lhs, const DataFormat &rhs);
};

using DataFormatPtr = std::shared_ptr<DataFormat>;

struct NotifyAttribute {
  const std::optional<uint16_t> minimum_period_;
  const std::optional<uint16_t> maximum_period_;
  const std::optional<double> greater_than_;
  const std::optional<double> less_than_;
  const std::optional<double> step_;
  const std::optional<uint16_t> minimum_evaluation_period_;
  const std::optional<uint16_t> maximum_evaluation_period_;

  NotifyAttribute(
      std::optional<uint16_t> minimum_period = std::nullopt,
      std::optional<uint16_t> maximum_period = std::nullopt,
      std::optional<double> greater_than = std::nullopt,
      std::optional<double> less_than = std::nullopt,
      std::optional<double> step = std::nullopt,
      std::optional<uint16_t> minimum_evaluation_period = std::nullopt,
      std::optional<uint16_t> maximum_evaluation_period = std::nullopt);

  size_t size();
};

using NotifyAttributePtr = std::shared_ptr<NotifyAttribute>;
using TargetAttribute = std::pair<ElmentIdVariant, NotifyAttributePtr>;
using TargetContent = std::pair<ElmentIdVariant, DataFormatPtr>;
using TargetContentVector = std::vector<TargetContent>;
using PayloadData =
    std::variant<DataFormatPtr, TargetContent, TargetContentVector,
                 ElmentIdVariant, std::vector<ElmentIdVariant>,
                 std::vector<TargetAttribute>>;

struct Payload {
  const PayloadData data_;
  const MediaType media_type_;

  Payload(DataFormatPtr data);
  Payload(TargetContentVector data);
  Payload(TargetContent data);
  Payload(ElmentIdVariant data);
  Payload(std::vector<ElmentIdVariant> data);
  Payload(std::vector<TargetAttribute> data);
  Payload(PayloadData data, MediaType format = MediaType::NOT_SPECIFIED);

  size_t size();
};

using PayloadPtr = std::shared_ptr<Payload>;
} // namespace LwM2M

#endif //__LWM2M_DATA_FORMAT_TYPE_HPP