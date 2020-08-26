#include "LwM2M_ObjectLink.hpp"

namespace LwM2M {
ObjectLink::ObjectLink(uint16_t object_id, uint16_t instance_id)
    : object_id_(object_id), instance_id_(instance_id) {}
} // namespace LwM2M
