#ifndef __LWM2M_REGISTRATION_INTERFACE_DEVICE_META_INFO_HPP
#define __LWM2M_REGISTRATION_INTERFACE_DEVICE_META_INFO_HPP

#include "ModelType.hpp"

#include <optional>
#include <unordered_map>
#include <vector>

namespace LwM2M {
/**
 * @addtogroup Message_Model
 * @{
 */

/**
 * @brief Wraps all of the available Client information
 *
 * Used by RegistrationInterfaceRequest messages
 *
 */
struct DeviceMetaInfo {
  using ObjectInstancesMap =
      std::unordered_map<unsigned int, std::vector<unsigned int>>;

  std::optional<size_t> life_time_;
  ObjectInstancesMap object_instances_map_;
  std::optional<std::string> endpoint_name_;
  std::optional<LwM2M_Version> version_;
  std::optional<BindingType> binding_;
  std::optional<bool> queue_mode_;
  std::optional<std::string> sms_number_;

  DeviceMetaInfo(std::optional<size_t> life_time = std::nullopt,
      ObjectInstancesMap object_instances_map = ObjectInstancesMap(),
      std::optional<std::string> endpoint_name = std::nullopt,
      std::optional<LwM2M_Version> version = LwM2M_Version::V1_0,
      std::optional<BindingType> binding = std::nullopt,
      std::optional<bool> queue_mode = std::nullopt,
      std::optional<std::string> sms_number = std::nullopt);
};

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_REGISTRATION_INTERFACE_DEVICE_META_INFO_HPP