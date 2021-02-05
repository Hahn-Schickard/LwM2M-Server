#ifndef __LWM2M_MODEL_ELEMENT_ID_HPP
#define __LWM2M_MODEL_ELEMENT_ID_HPP

#include <cstdint>
#include <variant>

namespace LwM2M {
struct ObjectID {
  const uint32_t id_;

  ObjectID() : id_(0) {}
  ObjectID(uint32_t id) : id_(id) {}
};

struct ObjectInstanceID {
  const ObjectID object_;
  const uint32_t id_;

  ObjectInstanceID() : object_(), id_(0) {}
  ObjectInstanceID(ObjectID object_id, uint32_t instance_id)
      : object_(object_id), id_(instance_id) {}

  ObjectInstanceID(uint32_t object_id, uint32_t instance_id)
      : ObjectInstanceID(ObjectID(object_id), instance_id) {}
};

struct ResourceID {
  const ObjectInstanceID object_instance_;
  const uint32_t id_;

  ResourceID() : object_instance_(), id_(0) {}
  ResourceID(ObjectInstanceID object_instance_id, uint32_t resource_id)
      : object_instance_(object_instance_id), id_(resource_id) {}

  ResourceID(ObjectID object_id, uint32_t object_instance_id,
             uint32_t resource_id)
      : ResourceID(ObjectInstanceID(object_id, object_instance_id),
                   resource_id) {}

  ResourceID(uint32_t object_id, uint32_t object_instance_id,
             uint32_t resource_id)
      : ResourceID(ObjectInstanceID(object_id, object_instance_id),
                   resource_id) {}
};

struct ResourceInstanceID {
  const ResourceID resource_;
  const uint32_t id_;

  ResourceInstanceID() : resource_(), id_(0) {}
  ResourceInstanceID(ResourceID resource_id, uint32_t resource_instance_id)
      : resource_(resource_id), id_(resource_instance_id) {}

  ResourceInstanceID(ObjectInstanceID object_instance_id, uint32_t resource_id,
                     uint32_t resource_instance_id)
      : ResourceInstanceID(ResourceID(object_instance_id, resource_id),
                           resource_instance_id) {}

  ResourceInstanceID(ObjectID object_id, uint32_t object_instance_id,
                     uint32_t resource_id, uint32_t resource_instance_id)
      : ResourceInstanceID(
            ResourceID(object_id, object_instance_id, resource_id),
            resource_instance_id) {}

  ResourceInstanceID(uint32_t object_id, uint32_t object_instance_id,
                     uint32_t resource_id, uint32_t resource_instance_id)
      : ResourceInstanceID(
            ResourceID(object_id, object_instance_id, resource_id),
            resource_instance_id) {}
};

using ElmentIdVariant =
    std::variant<ObjectID, ObjectInstanceID, ResourceID, ResourceInstanceID>;
} // namespace LwM2M

#endif //__LWM2M_MODEL_ELEMENT_ID_HPP