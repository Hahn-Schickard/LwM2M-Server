#include "Object.hpp"

#include <optional>

using namespace std;

namespace LwM2M {

using ResourceDescriptorMap = unordered_map<uint32_t, ResourceDescriptorPtr>;

ResourceDescriptorMap assignResourceDescriptors(
    ElementIDs requrired, ResourceDescriptorMap supported) {
  ResourceDescriptorMap result;
  // Assign mandatory resources first
  for (auto resource : supported) {
    if (resource.second->mandatory_) {
      result.emplace(resource);
    }
  }

  for (auto resource : requrired) {
    try {
      auto it = supported.find(resource.getResourceID());
      // Check if resource is supported and not previously assigned
      if (it != supported.end() && result.find(it->first) == result.end()) {
        // ignore multiple resource instances
        if (!it->second->multiple_instances_) {
          result.emplace(*it);
        }
      }
    } catch (bad_optional_access& ex) {
      // silently ignore element ids without any resources
    }
  }
  return result;
}

Object::Object(Observable::ExceptionHandler handler,
    RequesterInterfaceFacadePtr requester, EndpointPtr endpoint,
    RequiredObjectInstances instances, ObjectDescriptorPtr descriptor)
    : requester_(requester), endpoint_(endpoint), descriptor_(descriptor) {
  for (auto instance : instances) {
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

ObjectDescriptorPtr Object::getDescriptor() { return descriptor_; }

ObjectInstancePtr Object::getObjectInstance(ElementID id) {
  auto it = instances_.find(id.getObjectInstanceID());
  if (it != instances_.end()) {
    return it->second;
  } else {
    throw ObjectInstanceDoesNotExist(id);
  }
}

ObjectInstances Object::getObjectInstances() { return instances_; }

ResourcePtr Object::getResource(ElementID id) {
  return getObjectInstance(id)->getResource(id);
}

Resources Object::getResources(ElementID id) {
  return getObjectInstance(id)->getResources();
}

ResourceInstance Object::getResourceInstance(ElementID id) {
  return getResource(id)->getResourceInstance(id);
}

ResourceInstances Object::getResourceInstances(ElementID id) {
  return getResource(id)->getResourceInstances();
}
} // namespace LwM2M
