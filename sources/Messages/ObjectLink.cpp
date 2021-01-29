#include "ObjectLink.hpp"

#include <limits>

using namespace std;

namespace LwM2M {

ObjectLink::ObjectLink()
    : ObjectLink(numeric_limits<uint16_t>::max(),
                 numeric_limits<uint16_t>::max()) {}

ObjectLink::ObjectLink(uint16_t object_id, uint16_t instance_id)
    : object_id_(object_id), instance_id_(instance_id) {}
} // namespace LwM2M
