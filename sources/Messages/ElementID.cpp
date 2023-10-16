#include "ElementID.hpp"

#include <stdexcept>

using namespace std;

namespace LwM2M {

ElementID::ElementID(uint16_t object) : object_(object) {}

ElementID::ElementID(uint16_t object, uint16_t object_instance)
    : object_(object), object_instance_(object_instance) {}

ElementID::ElementID(
    uint16_t object, uint16_t object_instance, uint16_t resource)
    : object_(object), object_instance_(object_instance), resource_(resource) {}

ElementID::ElementID(uint16_t object, uint16_t object_instance,
    uint16_t resource, uint16_t resource_instance)
    : object_(object), object_instance_(object_instance), resource_(resource),
      resource_instance_(resource_instance) {}

ElementID::ElementID(const ElementID& id, uint16_t sub_id)
    : ElementID(id.getObjectID()) {
  if (id.hasObjectInstanceID()) {
    object_instance_ = id.getObjectInstanceID();
    if (id.hasResourceID()) {
      resource_ = id.getResourceID();
      if (id.hasResourceInstanceID()) {
        auto error_msg = "ID " + to_string(sub_id) +
            " could not be assigned. All ElementID values already assigned";
        throw invalid_argument(error_msg);
      } else {
        resource_instance_ = sub_id;
      }
    } else {
      resource_ = sub_id;
    }
  } else {
    object_instance_ = sub_id;
  }
}

uint16_t ElementID::getObjectID() const { return object_; }

uint16_t ElementID::getObjectInstanceID() const {
  if (hasObjectInstanceID()) {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    return object_instance_.value();
  } else {
    throw logic_error("ElementID " + toString() + " has no Object Instance ID");
  }
}

bool ElementID::hasObjectInstanceID() const {
  return object_instance_.has_value();
}

uint16_t ElementID::getResourceID() const {
  if (hasResourceID()) {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    return resource_.value();
  } else {
    throw logic_error("ElementID " + toString() + " has no Resource ID");
  }
}

bool ElementID::hasResourceID() const { return resource_.has_value(); }

uint16_t ElementID::getResourceInstanceID() const {
  if (hasResourceInstanceID()) {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    return resource_instance_.value();
  } else {
    throw logic_error(
        "ElementID " + toString() + " has no Resource Instance ID");
  }
}

bool ElementID::hasResourceInstanceID() const {
  return resource_instance_.has_value();
}

string ElementID::toString() const {
  auto result = to_string(object_);
  if (object_instance_) {
    result += '/' + to_string(object_instance_.value());
    if (resource_) {
      result += '/' + to_string(resource_.value());
      if (resource_instance_) {
        result += '/' + to_string(resource_instance_.value());
      }
    }
  }
  return result;
}

vector<string> ElementID::toStrings() const {
  vector<string> result = {to_string(object_)};
  if (object_instance_) {
    result.emplace_back(to_string(object_instance_.value()));
    if (resource_) {
      result.emplace_back(to_string(resource_.value()));
      if (resource_instance_) {
        result.emplace_back(to_string(resource_instance_.value()));
      }
    }
  }
  return result;
}

size_t ElementID::hash() const {
  size_t hash_value = object_;
  if (object_instance_) {
    hash_value <<= sizeof(object_instance_.value()) * 2;
    hash_value |= object_instance_.value();
    if (resource_) {
      hash_value <<= sizeof(resource_.value()) * 2;
      hash_value |= resource_.value();
      if (resource_instance_) {
        hash_value <<= sizeof(resource_instance_.value()) * 2;
        hash_value |= resource_instance_.value();
      }
    }
  }
  return hash_value;
}

size_t ElementID::size() const {
  size_t result = sizeof(object_);
  if (object_instance_) {
    result += sizeof(object_instance_.value());
    if (resource_) {
      result += sizeof(resource_.value());
      if (resource_instance_) {
        result += sizeof(resource_instance_.value());
      }
    }
  }
  return result;
}

bool operator==(const ElementID& lhs, const ElementID& rhs) {
  return (lhs.hash() == rhs.hash());
}

bool operator!=(const ElementID& lhs, const ElementID& rhs) {
  return !(lhs == rhs);
}

bool operator<(const ElementID& lhs, const ElementID& rhs) {
  return (lhs.hash() < rhs.hash());
}

bool operator>(const ElementID& lhs, const ElementID& rhs) { return rhs < lhs; }

bool operator<=(const ElementID& lhs, const ElementID& rhs) {
  return !(lhs > rhs);
}

bool operator>=(const ElementID& lhs, const ElementID& rhs) {
  return !(lhs < rhs);
}

} // namespace LwM2M

// NOLINTNEXTLINE
namespace std {
std::size_t hash<LwM2M::ElementID>::operator()(
    const LwM2M::ElementID& id) const {
  return id.hash();
}
} // namespace std
