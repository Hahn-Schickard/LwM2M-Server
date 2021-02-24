#include "ElementID.hpp"
#include "Variant_Visitor.hpp"

using namespace std;

namespace LwM2M {

ObjectID::ObjectID(uint16_t id) : id_(id) {}

string ObjectID::toString() const { return to_string(id_); }

vector<string> ObjectID::toStrings() const {
  return vector<string>{toString()};
}

bool operator==(const ObjectID &lhs, const ObjectID &rhs) {
  return lhs.id_ == rhs.id_ ? true : false;
}

ObjectInstanceID::ObjectInstanceID(ObjectID object_id, uint16_t instance_id)
    : object_(object_id), id_(instance_id) {}

ObjectInstanceID::ObjectInstanceID(uint16_t object_id, uint16_t instance_id)
    : ObjectInstanceID(ObjectID(object_id), instance_id) {}

string ObjectInstanceID::toString() const { return to_string(id_); }

vector<string> ObjectInstanceID::toStrings() const {
  auto buffer = object_.toStrings();
  buffer.push_back(toString());
  return buffer;
}

bool operator==(const ObjectInstanceID &lhs, const ObjectInstanceID &rhs) {
  if (lhs.object_ == rhs.object_ && lhs.id_ == rhs.id_) {
    return true;
  } else {
    return false;
  }
}

ResourceID::ResourceID(ObjectInstanceID object_instance_id,
                       uint16_t resource_id)
    : object_instance_(object_instance_id), id_(resource_id) {}

ResourceID::ResourceID(ObjectID object_id, uint16_t object_instance_id,
                       uint16_t resource_id)
    : ResourceID(ObjectInstanceID(object_id, object_instance_id), resource_id) {
}

ResourceID::ResourceID(uint16_t object_id, uint16_t object_instance_id,
                       uint16_t resource_id)
    : ResourceID(ObjectInstanceID(object_id, object_instance_id), resource_id) {
}

string ResourceID::toString() const { return to_string(id_); }

vector<string> ResourceID::toStrings() const {
  auto buffer = object_instance_.toStrings();
  buffer.push_back(toString());
  return buffer;
}

bool operator==(const ResourceID &lhs, const ResourceID &rhs) {
  if (lhs.object_instance_ == rhs.object_instance_ && lhs.id_ == rhs.id_) {
    return true;
  } else {
    return false;
  }
}

ResourceInstanceID::ResourceInstanceID(ResourceID resource_id,
                                       uint16_t resource_instance_id)
    : resource_(resource_id), id_(resource_instance_id) {}

ResourceInstanceID::ResourceInstanceID(ObjectInstanceID object_instance_id,
                                       uint16_t resource_id,
                                       uint16_t resource_instance_id)
    : ResourceInstanceID(ResourceID(object_instance_id, resource_id),
                         resource_instance_id) {}

ResourceInstanceID::ResourceInstanceID(ObjectID object_id,
                                       uint16_t object_instance_id,
                                       uint16_t resource_id,
                                       uint16_t resource_instance_id)
    : ResourceInstanceID(ResourceID(object_id, object_instance_id, resource_id),
                         resource_instance_id) {}

ResourceInstanceID::ResourceInstanceID(uint16_t object_id,
                                       uint16_t object_instance_id,
                                       uint16_t resource_id,
                                       uint16_t resource_instance_id)
    : ResourceInstanceID(ResourceID(object_id, object_instance_id, resource_id),
                         resource_instance_id) {}

string ResourceInstanceID::toString() const { return to_string(id_); }

vector<string> ResourceInstanceID::toStrings() const {
  auto buffer = resource_.toStrings();
  buffer.push_back(toString());
  return buffer;
}

bool operator==(const ResourceInstanceID &lhs, const ResourceInstanceID &rhs) {
  if (lhs.resource_ == rhs.resource_ && lhs.id_ == rhs.id_) {
    return true;
  } else {
    return false;
  }
}

size_t size_of(ElmentIdVariant value) {
  size_t result = 0;
  match(value, [&](ObjectID /*target*/) { result = 4; },
        [&](ObjectInstanceID /*target*/) { result = 8; },
        [&](ResourceID /*target*/) { result = 12; },
        [&](ResourceInstanceID /*target*/) { result = 16; });

  return result;
}

vector<string> toStrings(ElmentIdVariant value) {
  vector<string> result;
  match(value, [&](ObjectID target) { result = target.toStrings(); },
        [&](ObjectInstanceID target) { result = target.toStrings(); },
        [&](ResourceID target) { result = target.toStrings(); },
        [&](ResourceInstanceID target) { result = target.toStrings(); });

  return result;
}
} // namespace LwM2M
