#include "ObjectDescriptor.hpp"

using namespace std;
namespace LwM2M {

ObjectDescriptor::ObjectDescriptor()
    : name_(string()), description_(string()), id_(0),
      multiple_instances_(false), mandatory_(false), urn_(string()),
      resources_() {}

ObjectDescriptor::ObjectDescriptor(ObjectDescriptor&& instance) noexcept
    : name_(instance.name_), description_(instance.description_),
      id_(instance.id_), multiple_instances_(instance.multiple_instances_),
      mandatory_(instance.mandatory_), urn_(instance.urn_),
      resources_(instance.resources_) {}

ObjectDescriptor::ObjectDescriptor(string name, string description, uint32_t id,
    bool multiple_instances, bool mandatory, string urn,
    unordered_map<uint32_t, shared_ptr<ResourceDescriptor>> resources) // NOLINT
    : name_(move(name)), description_(move(description)), id_(id),
      multiple_instances_(multiple_instances), mandatory_(mandatory),
      urn_(move(urn)), resources_(resources) {} // NOLINT

ResourceDescriptorPtr ObjectDescriptor::getResource(uint32_t id) {
  auto it = resources_.find(id);
  if (it != resources_.end()) {
    return it->second;
  } else {
    throw ResourceDescriptorDoesNotExist(this->id_, id);
  }
}
} // namespace LwM2M