#include "Resource.hpp"

#include <future>
using namespace std;

namespace LwM2M {

ResourceInstance makeVariant(const ResourceDescriptorPtr& descriptor,
    Observable::ExceptionHandler handler, RequesterInterfaceFacadePtr requester,
    EndpointPtr endpoint, ElementID id) {
  switch (descriptor->operations_) {
  case OperationsType::READ: {
    return NonemptyPointer::make_shared<Readable>(
        handler, requester, requester, endpoint, id, descriptor->data_type_);
  }
  case OperationsType::WRITE: {
    return NonemptyPointer::make_shared<Writable>(requester, endpoint, id);
  }
  case OperationsType::READ_AND_WRITE: {
    return NonemptyPointer::make_shared<ReadAndWritable>(handler, requester,
        requester, requester, endpoint, id, descriptor->data_type_);
  }
  case OperationsType::EXECUTE: {
    return NonemptyPointer::make_shared<Executable>(requester, endpoint, id);
  }
  case OperationsType::NO_OPERATION:
  default: { return NonemptyPointer::make_shared<Operationless>(); }
  }
}

Resource::Resource(Observable::ExceptionHandler handler, // NOLINT
    RequesterInterfaceFacadePtr requester, EndpointPtr endpoint, // NOLINT
    ResourceDescriptorPtr descriptor, // NOLINT
    ElementID id, std::optional<uint16_t> instance_id)
    : descriptor_(descriptor), id_(id) { // NOLINT
  auto element_id = id;
  if (instance_id.has_value()) {
    element_id = ElementID(id, instance_id.value());
  }
  auto variant = makeVariant(
      descriptor, handler, requester, endpoint, element_id); // NOLINT
  instances_.emplace(element_id, variant);
}

ResourceDescriptorPtr Resource::getDescriptor() { return descriptor_; }

ResourceInstance Resource::getResourceInstance(bool ignore_multiple_instances) {
  if (instances_.size() > 1 && !ignore_multiple_instances) {
    throw ResourceInstanceCouldNotBeResolved(id_);
  } else {
    return instances_.begin()->second;
  }
}

ResourceInstance Resource::getResourceInstance(ElementID id) {
  auto it = instances_.find(id);
  if (it != instances_.end()) {
    return it->second;
  } else {
    throw ResourceInstanceDoesNotExist(id);
  }
}

ResourceInstances Resource::getResourceInstances() { return instances_; }
} // namespace LwM2M