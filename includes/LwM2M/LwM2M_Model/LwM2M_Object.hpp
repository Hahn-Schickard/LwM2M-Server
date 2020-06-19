#ifndef __LWM2M_MODEL_OBJECT_HPP
#define __LWM2M_MODEL_OBJECT_HPP

#include "LwM2M_Resource.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace LwM2M_Model {

struct LwM2M_Object {

  const std::string name_;
  const std::string description_;
  const uint32_t id_;
  const bool multiple_instances_;
  const bool mandatory_;
  const std::string urn_;
  const std::unordered_map<uint32_t, LwM2M_Resource> resources_;

  LwM2M_Object()
      : name_(std::string()), description_(std::string()), id_(0),
        multiple_instances_(false), mandatory_(false), urn_(std::string()),
        resources_() {}

  LwM2M_Object(const LwM2M_Object &instance)
      : name_(instance.name_), description_(instance.description_),
        id_(instance.id_), multiple_instances_(instance.multiple_instances_),
        mandatory_(instance.mandatory_), urn_(instance.urn_),
        resources_(instance.resources_) {}

  LwM2M_Object(LwM2M_Object &&instance)
      : name_(std::move(instance.name_)),
        description_(std::move(instance.description_)), id_(instance.id_),
        multiple_instances_(instance.multiple_instances_),
        mandatory_(instance.mandatory_), urn_(std::move(instance.urn_)),
        resources_(std::move(instance.resources_)) {}

  LwM2M_Object(std::string name, std::string description, uint32_t id,
               bool multiple_instances, bool mandatory, std::string urn,
               std::unordered_map<uint32_t, LwM2M_Resource> resources)
      : name_(std::move(name)), description_(std::move(description)), id_(id),
        multiple_instances_(multiple_instances), mandatory_(mandatory),
        urn_(std::move(urn)), resources_(std::move(resources)) {}
};

} // namespace LwM2M_Model

#endif //__LWM2M_MODEL_OBJECT_HPP