#include "Resource.hpp"

#include <future>
using namespace std;

namespace LwM2M {

ResourceInstance makeVariant(const ResourceDescriptorPtr& descriptor,
    const Observable::ExceptionHandler& handler,
    const RequesterInterfaceFacadePtr& requester, const EndpointPtr& endpoint,
    const ElementID& id) {
  switch (descriptor->operations_) {
  case OperationsType::READ: {
    return Nonempty::make_shared<Readable>(
        handler, requester, requester, endpoint, id, descriptor->data_type_);
  }
  case OperationsType::WRITE: {
    return Nonempty::make_shared<Writable>(requester, endpoint, id);
  }
  case OperationsType::READ_AND_WRITE: {
    return Nonempty::make_shared<ReadAndWritable>(handler, requester, requester,
        requester, endpoint, id, descriptor->data_type_);
  }
  case OperationsType::EXECUTE: {
    return Nonempty::make_shared<Executable>(requester, endpoint, id);
  }
  case OperationsType::NO_OPERATION:
  default: {
    return Nonempty::make_shared<Operationless>();
  }
  }
}

Resource::Resource(const Observable::ExceptionHandler& handler,
    const RequesterInterfaceFacadePtr& requester, const EndpointPtr& endpoint,
    const ResourceDescriptorPtr& descriptor, const ElementID& id,
    const optional<uint16_t>& instance_id)
    : descriptor_(descriptor), id_(id) {
  auto element_id = id;
  if (instance_id.has_value()) {
    element_id = ElementID(id, instance_id.value());
  }
  auto variant =
      makeVariant(descriptor, handler, requester, endpoint, element_id);
  instances_.emplace(element_id, variant);
}

ResourceDescriptorPtr Resource::getDescriptor() const { return descriptor_; }

ResourceInstance Resource::getResourceInstance(
    bool ignore_multiple_instances) const {
  if (instances_.size() > 1 && !ignore_multiple_instances) {
    throw ResourceInstanceCouldNotBeResolved(id_);
  } else {
    return instances_.begin()->second;
  }
}

ResourceInstance Resource::getResourceInstance(const ElementID& id) const {
  auto it = instances_.find(id);
  if (it != instances_.end()) {
    return it->second;
  } else {
    throw ResourceInstanceDoesNotExist(id);
  }
}

ResourceInstances Resource::getResourceInstances() const { return instances_; }
} // namespace LwM2M