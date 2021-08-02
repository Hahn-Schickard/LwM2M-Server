#include "Object.hpp"

using namespace std;

namespace LwM2M {

using ResourceDescriptorMap = unordered_map<uint32_t, ResourceDescriptorPtr>;

ResourceDescriptorMap
assignResourceDescriptors(ElementIDs requrired,
                          ResourceDescriptorMap supported) {
  ResourceDescriptorMap result;
  for (auto resource : requrired) {
    auto it = supported.find(resource.getResourceID());
    if (it != supported.end()) {
      result.emplace(*it);
    }
  }
  return result;
}

Object::Object(RequesterPtr requester, EndpointPtr endpoint,
               RequiredObjectInstances instances,
               ObjectDescriptorPtr descriptor)
    : requester_(requester), endpoint_(endpoint), descriptor_(descriptor) {
  for (auto instance : instances) {
    ElementIDs resources;
    auto range = instances.equal_range(instance);
    for (auto resource = range.first; resource != range.second; ++resource) {
      resources.emplace_back(*resource);
    }

    auto available_descriptors =
        assignResourceDescriptors(resources, descriptor_->resources_);
    instances_.emplace(
        instance.getObjectInstanceID(),
        make_shared<ObjectInstance>(
            requester_, endpoint,
            ElementID(instance.getObjectID(), instance.getObjectInstanceID()),
            available_descriptors));
  }
}

ResourceVariant Object::getResource(ElementID id) {
  auto it = instances_.find(id.getObjectInstanceID());
  if (it != instances_.end()) {
    auto object_instance = it->second;
    return object_instance->getResource(id);
  } else {
    throw ObjectInstanceDoesNotExist(id);
  }
}

ObjectDescriptorPtr Object::getDescriptor() { return descriptor_; }

ObjectInstances Object::getInstances() { return instances_; }
} // namespace LwM2M
