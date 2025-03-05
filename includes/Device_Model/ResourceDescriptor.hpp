#ifndef __LWM2M_MODEL_RESOURCE_DESCRIPTOR_HPP
#define __LWM2M_MODEL_RESOURCE_DESCRIPTOR_HPP

#include "DataFormat.hpp"
#include "RangeEnumeration.hpp"

#include <cstdint>
#include <memory>
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
  No_Operation, /*!< @see Operationless */
  Read, /*!< @see Readable */
  Write, /*!< @see Writable */
  Read_And_Write, /*!< @see ReadAndWritable */
  Execute /*!< @see Executable */
};

std::string toString(OperationsType operation);

/**
 * @brief Contains static information of a Resource or its instance.
 *
 * Used to cread a representation of a single data point that is modeled by the
 * LwM2M Resource Model
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

  /**
   * @brief Creates a default resource descriptor. Used for testing only
   *
   */
  ResourceDescriptor();

  /**
   * @brief Copy constructor
   *
   * @param instance
   */
  ResourceDescriptor(const ResourceDescriptor& instance) = default;

  /**
   * @brief Move constructor
   *
   * @param instance
   */
  ResourceDescriptor(ResourceDescriptor&& instance) noexcept;

  /**
   * @brief Creates a Resource descriptor without a range enumeration value
   *
   * @param id - resource id
   * @param name - resource name
   * @param operations - supported operation type
   * @param multiple_instances - does this resource support multiple instances
   * of itself?
   * @param mandatory  - is the resource required for a valid object instance?
   * @param data_type - modeled resource data type
   * @param units - human readable measurement of the modeled data type (i.e. °C
   * for temperature)
   * @param description - human readable description of this resource
   */
  ResourceDescriptor(uint32_t id, std::string name, OperationsType operations,
      bool multiple_instances, bool mandatory, DataType data_type,
      std::string units, std::string description);

  /**
   * @brief Creates a Resource descriptor with a given range enumeration value
   * of valid data points
   *
   * @param id - resource id
   * @param name - resource name
   * @param operations - supported operation type
   * @param multiple_instances - does the resource support multiple instances?
   * @param mandatory  - is the resource required for a valid object instance?
   * @param data_type - modeled resource data type
   * @param range_enumeration - a valid range of data values (this range can be
   * modeled as integers, decimal values or string literals)
   * @param units - human readable measurement of the modeled data type (i.e. °C
   * for temperature)
   * @param description - human readable description of this resource
   */
  ResourceDescriptor(uint32_t id, std::string name, OperationsType operations,
      bool multiple_instances, bool mandatory, DataType data_type,
      RangeEnumeration range_enumeration, std::string units,
      std::string description);
};

using ResourceDescriptorPtr = std::shared_ptr<ResourceDescriptor>;

} // namespace LwM2M
#endif //__LWM2M_MODEL_RESOURCE_DESCRIPTOR_HPP