#include "ObjectLink.hpp"

#include <limits>

using namespace std;

namespace LwM2M {

ObjectLink::ObjectLink()
    : ObjectLink(
          numeric_limits<uint16_t>::max(), numeric_limits<uint16_t>::max()) {}

ObjectLink::ObjectLink(uint16_t object_id, uint16_t instance_id)
    : object_id_(object_id), instance_id_(instance_id) {}

string ObjectLink::toString() const {
  return to_string(object_id_) + "/" + to_string(instance_id_);
}

bool operator==(const ObjectLink& lhs, const ObjectLink& rhs) {
  return (lhs.object_id_ == rhs.object_id_) &&
      (lhs.instance_id_ == rhs.instance_id_);
}

} // namespace LwM2M
