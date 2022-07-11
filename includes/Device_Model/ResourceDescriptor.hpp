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
  NO_OPERATION,
  READ,
  WRITE,
  READ_AND_WRITE,
  EXECUTE
};

std::string toString(OperationsType operation);

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
  ResourceDescriptor(const ResourceDescriptor& instance);
  ResourceDescriptor(ResourceDescriptor&& instance);
  ResourceDescriptor(uint32_t id, std::string name, OperationsType operations,
      bool multiple_instances, bool mandatory, DataType data_type,
      std::string units, std::string description);
  ResourceDescriptor(uint32_t id, std::string name, OperationsType operations,
      bool multiple_instances, bool mandatory, DataType data_type,
      RangeEnumeration range_enumeration, std::string units,
      std::string description);
};

using ResourceDescriptorPtr = std::shared_ptr<ResourceDescriptor>;

} // namespace LwM2M
#endif //__LWM2M_MODEL_RESOURCE_DESCRIPTOR_HPP