#ifndef __LWM2M_MODEL_OBJECT_DESCRIPTOR_HPP
#define __LWM2M_MODEL_OBJECT_DESCRIPTOR_HPP

#include "LwM2M_ResourceDescriptor.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace LwM2M {

struct ObjectDescriptor {

  const std::string name_;
  const std::string description_;
  const uint32_t id_;
  const bool multiple_instances_;
  const bool mandatory_;
  const std::string urn_;
  const std::unordered_map<uint32_t, std::shared_ptr<ResourceDescriptor>>
      resources_;

  ObjectDescriptor();
  ObjectDescriptor(const ObjectDescriptor &instance);
  ObjectDescriptor(ObjectDescriptor &&instance);
  ObjectDescriptor(
      std::string name, std::string description, uint32_t id,
      bool multiple_instances, bool mandatory, std::string urn,
      std::unordered_map<uint32_t, std::shared_ptr<ResourceDescriptor>>
          resources);

  std::shared_ptr<ResourceDescriptor> getResource(uint32_t id);
};

} // namespace LwM2M

#endif //__LWM2M_MODEL_OBJECT_DESCRIPTOR_HPP