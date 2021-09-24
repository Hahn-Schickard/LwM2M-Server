#include "DeviceMetaInfo.hpp"

using namespace std;

namespace LwM2M {

DeviceMetaInfo::DeviceMetaInfo(optional<size_t> life_time,
                               ObjectInstancesMap object_instances_map,
                               optional<string> endpoint_name,
                               optional<LwM2M_Version> version,
                               optional<BindingType> binding,
                               optional<bool> queue_mode,
                               optional<string> sms_number)
    : life_time_(life_time), object_instances_map_(object_instances_map),
      endpoint_name_(endpoint_name), version_(version), binding_(binding),
      queue_mode_(queue_mode), sms_number_(sms_number) {}

} // namespace LwM2M
