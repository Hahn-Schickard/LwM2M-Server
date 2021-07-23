#include "Object.hpp"

using namespace std;

namespace LwM2M {

using ResourceDescriptorMap = unordered_map<uint32_t, ResourceDescriptorPtr>;

ResourceDescriptorMap
assignResourceDescriptors(ObjectInstanceID::ResourceIDs requrired,
                          ResourceDescriptorMap supported) {
  ResourceDescriptorMap result;
  for (auto resource : requrired) {
    auto it = supported.find(resource.getID());
    if (it != supported.end()) {
      result.emplace(*it);
    }
  }
  return result;
}

Object::Object(RequesterPtr requester, EndpointPtr endpoint,
               ObjectID::ObjectInstanceIDs instances,
               ObjectDescriptorPtr descriptor)
    : requester_(requester), endpoint_(endpoint), descriptor_(descriptor) {
  for (auto instance : instances) {
    auto resources = instance.getResourceIDs();
    auto available_descriptors =
        assignResourceDescriptors(resources, descriptor_->resources_);
    instances_.emplace(
        instance.getID(),
        make_shared<ObjectInstance>(requester_, endpoint, descriptor->id_,
                                    instance.getID(), available_descriptors));
  }
}

ResourceVariant Object::getResource(ObjectInstanceID instance,
                                    ResourceID resource) {
  auto it = instances_.find(instance.getID());
  if (it != instances_.end()) {
    auto object_instance = it->second;
    return object_instance->getResource(resource.getID());
  } else {
    throw ObjectInstanceDoesNotExist(instance);
  }
}

ObjectDescriptorPtr Object::getDescriptor() { return descriptor_; }

ObjectInstances Object::getInstances() { return instances_; }
} // namespace LwM2M
