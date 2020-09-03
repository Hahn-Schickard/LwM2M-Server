#include "LwM2M_ObjectLink.hpp"

#define MAX_SHORT_VALUE 0xFFFF

namespace LwM2M {

ObjectLink::ObjectLink() : ObjectLink(MAX_SHORT_VALUE, MAX_SHORT_VALUE) {}

ObjectLink::ObjectLink(uint16_t object_id, uint16_t instance_id)
    : object_id_(object_id), instance_id_(instance_id) {}
} // namespace LwM2M
