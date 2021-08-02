#include "ElementID.hpp"

using namespace std;

namespace LwM2M {

ElementID::ElementID(uint16_t object) : object_(object) {}

ElementID::ElementID(uint16_t object, uint16_t object_instance)
    : object_(object), object_instance_(object_instance) {}

ElementID::ElementID(uint16_t object, uint16_t object_instance,
                     uint16_t resource)
    : object_(object), object_instance_(object_instance), resource_(resource) {}

ElementID::ElementID(uint16_t object, uint16_t object_instance,
                     uint16_t resource, uint16_t resource_instance)
    : object_(object), object_instance_(object_instance), resource_(resource),
      resource_instance_(resource_instance) {}

uint16_t ElementID::getObjectID() const { return object_; }

uint16_t ElementID::getObjectInstanceID() const { return *object_instance_; }

uint16_t ElementID::getResourceID() const { return *resource_; }

uint16_t ElementID::getResourceInstanceID() const {
  return *resource_instance_;
}

string ElementID::toString() const {
  auto result = to_string(object_);
  if (object_instance_) {
    result += '/' + to_string(*object_instance_);
    if (resource_) {
      result += '/' + to_string(*resource_);
      if (resource_instance_) {
        result += '/' + to_string(*resource_instance_);
      }
    }
  }
  return result;
}

vector<string> ElementID::toStrings() const {
  vector<string> result = {to_string(object_)};
  if (object_instance_) {
    result.emplace_back(to_string(*object_instance_));
    if (resource_) {
      result.emplace_back(to_string(*resource_));
      if (resource_instance_) {
        result.emplace_back(to_string(*resource_instance_));
      }
    }
  }
  return result;
}

size_t ElementID::hash() const {
  size_t hash_value = object_;
  if (object_instance_) {
    hash_value <<= sizeof(*object_instance_) * 2;
    hash_value |= *object_instance_;
    if (resource_) {
      hash_value <<= sizeof(*resource_) * 2;
      hash_value |= *resource_;
      if (resource_instance_) {
        hash_value <<= sizeof(*resource_instance_) * 2;
        hash_value |= *resource_instance_;
      }
    }
  }
  return hash_value;
}

size_t ElementID::size() const {
  size_t result = sizeof(object_);
  if (object_instance_) {
    result += sizeof(*object_instance_);
    if (resource_) {
      result += sizeof(*resource_);
      if (resource_instance_) {
        result += sizeof(*resource_instance_);
      }
    }
  }
  return result;
}

bool operator==(const ElementID &lhs, const ElementID &rhs) {
  return (lhs.hash() == rhs.hash());
}

} // namespace LwM2M

namespace std {
std::size_t hash<LwM2M::ElementID>::
operator()(const LwM2M::ElementID &id) const {
  return id.hash();
}
} // namespace std
