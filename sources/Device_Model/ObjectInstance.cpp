#include "ObjectInstance.hpp"

#include "Executable.hpp"
#include "ReadAndWritable.hpp"
#include "Readable.hpp"
#include "Writable.hpp"

using namespace std;

namespace LwM2M {

ObjectInstance::ObjectInstance(const Observable::ExceptionHandler& handler,
    const RequesterInterfaceFacadePtr& requester, const EndpointPtr& endpoint,
    const ElementID& id,
    const unordered_map<uint32_t, ResourceDescriptorPtr>& resource_descriptors)
    : requester_(requester), endpoint_(endpoint), id_(id) {
  for (const auto& resource_pair : resource_descriptors) {
    resources_.emplace(resource_pair.first,
        make_shared<Resource>(handler, requester, endpoint,
            resource_pair.second,
            ElementID(id.getObjectID(), id.getObjectInstanceID(),
                resource_pair.first)));
  }
}

ElementID ObjectInstance::getId() const { return id_; }

ResourcePtr ObjectInstance::getResource(const ElementID& id) const {
  auto it = resources_.find(id.getResourceID());
  if (it != resources_.end()) {
    return it->second;
  } else {
    throw ResourceDoesNotExist(id);
  }
}

Resources ObjectInstance::getResources() const { return resources_; }

ResourceInstance ObjectInstance::getResourceInstance(
    const ElementID& id) const {
  return getResource(id)->getResourceInstance(id);
}

ResourceInstances ObjectInstance::getResourceInstances(
    const ElementID& id) const {
  return getResource(id)->getResourceInstances();
}

} // namespace LwM2M