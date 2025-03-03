#include "Object.hpp"

#include <optional>

using namespace std;

namespace LwM2M {

using ResourceDescriptorMap = unordered_map<uint32_t, ResourceDescriptorPtr>;

ResourceDescriptorMap assignResourceDescriptors(
    const ElementIDs& required, const ResourceDescriptorMap& supported) {
  ResourceDescriptorMap result;
  // Assign mandatory resources first
  for (const auto& resource : supported) {
    if (resource.second->mandatory_) {
      result.emplace(resource);
    }
  }

  for (const auto& resource : required) {
    try {
      auto it = supported.find(resource.getResourceID());
      // Check if resource is supported and not previously assigned
      if (it != supported.end() && result.find(it->first) == result.end()) {
        // ignore multiple resource instances
        if (!it->second->multiple_instances_) {
          result.emplace(*it);
        }
      }
    } catch (const MissingOptionalID& ex) { // NOLINT(bugprone-empty-catch)
      // silently ignore element ids without any resources
    }
  }
  return result;
}

Object::Object(const Observable::ExceptionHandler& handler,
    const RequesterInterfaceFacadePtr& requester, const EndpointPtr& endpoint,
    const RequiredObjectInstances& instances,
    const ObjectDescriptorPtr& descriptor)
    : requester_(requester), endpoint_(endpoint), descriptor_(descriptor) {
  for (const auto& instance : instances) {
    ElementIDs resources;
    auto range = instances.equal_range(instance);
    for (auto resource = range.first; resource != range.second; ++resource) {
      resources.emplace_back(*resource);
    }

    auto available_descriptors =
        assignResourceDescriptors(resources, descriptor_->resources_);
    instances_.emplace(instance.getObjectInstanceID(),
        make_shared<ObjectInstance>(handler, requester_, endpoint,
            ElementID(instance.getObjectID(), instance.getObjectInstanceID()),
            available_descriptors));
  }
}

ObjectDescriptorPtr Object::getDescriptor() const { return descriptor_; }

ObjectInstancePtr Object::getObjectInstance(const ElementID& id) const {
  auto it = instances_.find(id.getObjectInstanceID());
  if (it != instances_.end()) {
    return it->second;
  } else {
    throw ObjectInstanceDoesNotExist(id);
  }
}

ObjectInstances Object::getObjectInstances() const { return instances_; }

ResourcePtr Object::getResource(const ElementID& id) const {
  return getObjectInstance(id)->getResource(id);
}

Resources Object::getResources(const ElementID& id) const {
  return getObjectInstance(id)->getResources();
}

ResourceInstance Object::getResourceInstance(const ElementID& id) const {
  return getResource(id)->getResourceInstance(id);
}

ResourceInstances Object::getResourceInstances(const ElementID& id) const {
  return getResource(id)->getResourceInstances();
}
} // namespace LwM2M
