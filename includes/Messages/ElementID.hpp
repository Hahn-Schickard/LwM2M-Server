#ifndef __LWM2M_MODEL_ELEMENT_ID_HPP
#define __LWM2M_MODEL_ELEMENT_ID_HPP

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace LwM2M {
struct ObjectID {
  const uint16_t id_ = 0;

  ObjectID() = default;
  ObjectID(uint16_t id);

  std::string toString() const;
  std::vector<std::string> toStrings() const;

  friend bool operator==(const ObjectID &lhs, const ObjectID &rhs);
};

struct ObjectInstanceID {
  const ObjectID object_ = ObjectID();
  const uint16_t id_ = 0;

  ObjectInstanceID() = default;
  ObjectInstanceID(ObjectID object_id, uint16_t instance_id);
  ObjectInstanceID(uint16_t object_id, uint16_t instance_id);

  std::string toString() const;
  std::vector<std::string> toStrings() const;

  friend bool operator==(const ObjectInstanceID &lhs,
                         const ObjectInstanceID &rhs);
};

struct ResourceID {
  const ObjectInstanceID object_instance_ = ObjectInstanceID();
  const uint16_t id_ = 0;

  ResourceID() = default;
  ResourceID(ObjectInstanceID object_instance_id, uint16_t resource_id);
  ResourceID(ObjectID object_id, uint16_t object_instance_id,
             uint16_t resource_id);
  ResourceID(uint16_t object_id, uint16_t object_instance_id,
             uint16_t resource_id);

  std::string toString() const;
  std::vector<std::string> toStrings() const;

  friend bool operator==(const ResourceID &lhs, const ResourceID &rhs);
};

struct ResourceInstanceID {
  const ResourceID resource_ = ResourceID();
  const uint16_t id_ = 0;

  ResourceInstanceID() = default;
  ResourceInstanceID(ResourceID resource_id, uint16_t resource_instance_id);
  ResourceInstanceID(ObjectInstanceID object_instance_id, uint16_t resource_id,
                     uint16_t resource_instance_id);
  ResourceInstanceID(ObjectID object_id, uint16_t object_instance_id,
                     uint16_t resource_id, uint16_t resource_instance_id);
  ResourceInstanceID(uint16_t object_id, uint16_t object_instance_id,
                     uint16_t resource_id, uint16_t resource_instance_id);

  std::string toString() const;
  std::vector<std::string> toStrings() const;

  friend bool operator==(const ResourceInstanceID &lhs,
                         const ResourceInstanceID &rhs);
};

using ElmentIdVariant =
    std::variant<ObjectID, ObjectInstanceID, ResourceID, ResourceInstanceID>;

size_t size_of(ElmentIdVariant value);

std::vector<std::string> toStrings(ElmentIdVariant value);
} // namespace LwM2M

#endif //__LWM2M_MODEL_ELEMENT_ID_HPP