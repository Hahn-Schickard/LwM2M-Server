#include "Object.hpp"

using namespace std;

namespace LwM2M {

Object::Object(RequesterPtr requester, EndpointPtr endpoint,
               vector<uint32_t> instances, ObjectDescriptorPtr descriptor)
    : requester_(requester), endpoint_(endpoint), descriptor_(descriptor) {
  for (auto instance : instances) {
    instances_.emplace(
        instance,
        make_shared<ObjectInstance>(requester_, endpoint,
                                    ObjectInstanceID(descriptor->id_, instance),
                                    descriptor_->resources_));
  }
}

ResourceVariant Object::getResource(uint32_t instance_id,
                                    uint32_t resource_id) {
  auto it = instances_.find(instance_id);
  if (it != instances_.end()) {
    auto object_instance = it->second;
    return object_instance->getResource(resource_id);
  } else {
    return ResourceVariant();
  }
}

ObjectDescriptorPtr Object::getDescriptor() { return descriptor_; }

ObjectInstances Object::getInstances() { return instances_; }
} // namespace LwM2M
