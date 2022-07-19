#include "ObjectInstance.hpp"

#include "Executable.hpp"
#include "ReadAndWritable.hpp"
#include "Readable.hpp"
#include "Writable.hpp"

using namespace std;

namespace LwM2M {

ObjectInstance::ObjectInstance(Observable::ExceptionHandler handler,
    RequesterPtr requester, EndpointPtr endpoint, ElementID id,
    unordered_map<uint32_t, ResourceDescriptorPtr> resource_descriptors)
    : requester_(requester), endpoint_(endpoint), id_(id) {
  for (auto resource_pair : resource_descriptors) {
    resources_.emplace(resource_pair.first,
        make_shared<Resource>(handler, requester, endpoint,
            resource_pair.second,
            ElementID(id.getObjectID(), id.getObjectInstanceID(),
                resource_pair.first)));
  }
}

ElementID ObjectInstance::getId() { return id_; }

ResourcePtr ObjectInstance::getResource(ElementID id) {
  auto it = resources_.find(id.getResourceID());
  if (it != resources_.end()) {
    return it->second;
  } else {
    throw ResourceDoesNotExist(id);
  }
}

Resources ObjectInstance::getResources() { return resources_; }

} // namespace LwM2M