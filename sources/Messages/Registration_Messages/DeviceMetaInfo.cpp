#include "DeviceMetaInfo.hpp"

using namespace std;

namespace LwM2M {

DeviceMetaInfo::DeviceMetaInfo(optional<size_t> life_time, // NOLINT
    ObjectInstancesMap object_instances_map, // NOLINT
    optional<string> endpoint_name, // NOLINT
    optional<LwM2M_Version> version, // NOLINT
    optional<BindingType> binding, // NOLINT
    optional<bool> queue_mode, // NOLINT
    optional<string> sms_number) // NOLINT
    : life_time_(life_time),
      object_instances_map_(object_instances_map), // NOLINT
      endpoint_name_(endpoint_name), version_(version), // NOLINT
      binding_(binding), // NOLINT
      queue_mode_(queue_mode), sms_number_(sms_number) {} // NOLINT

} // namespace LwM2M
