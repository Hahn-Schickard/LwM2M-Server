#include "ObjectInstance.hpp"

#include "Executable.hpp"
#include "ReadAndWritable.hpp"
#include "Readable.hpp"
#include "Writable.hpp"

using namespace std;

namespace LwM2M {

template <typename T>
shared_ptr<Resource<T>>
makeResourcePtr(RequesterPtr requester, EndpointPtr endpoint,
                ObjectInstanceID id, ResourceDescriptorPtr descriptor) {
  switch (descriptor->operations_) {
  case OperationsType::READ: {
    return make_shared<Readable<T>>(requester, endpoint, id, descriptor);
  }
  case OperationsType::WRITE: {
    return make_shared<Writable<T>>(requester, endpoint, id, descriptor);
  }
  case OperationsType::READ_AND_WRITE: {
    return make_shared<ReadAndWritable<T>>(requester, endpoint, id, descriptor);
  }
  case OperationsType::EXECUTE: {
    return make_shared<Executable<T>>(requester, endpoint, id, descriptor);
  }
  case OperationsType::NO_OPERATION:
  default: { return make_shared<Resource<T>>(); }
  }
}

ObjectInstance::ObjectInstance(
    RequesterPtr requester, EndpointPtr endpoint, ObjectInstanceID id,
    unordered_map<uint32_t, shared_ptr<ResourceDescriptor>>
        resource_descriptors)
    : requester_(requester), endpoint_(endpoint), id_(id) {
  for (auto resource_pair : resource_descriptors) {
    if (resource_pair.second->mandatory_ &&
        (resource_pair.second->multiple_instances_ == false)) {
      switch (resource_pair.second->data_type_) {
      case DataType::FLOAT: {
        resources_.emplace(resource_pair.first,
                           makeResourcePtr<double>(requester_, endpoint, id_,
                                                   resource_pair.second));
        break;
      }
      case DataType::SIGNED_INTEGER: {
        resources_.emplace(resource_pair.first,
                           makeResourcePtr<int64_t>(requester_, endpoint, id_,
                                                    resource_pair.second));
        break;
      }
      case DataType::OBJECT_LINK: {
        resources_.emplace(resource_pair.first, makeResourcePtr<ObjectLink>(
                                                    requester_, endpoint, id_,
                                                    resource_pair.second));
        break;
      }
      case DataType::OPAQUE: {
        resources_.emplace(
            resource_pair.first,
            makeResourcePtr<vector<uint8_t>>(requester_, endpoint, id_,
                                             resource_pair.second));
        break;
      }
      case DataType::STRING: {
        resources_.emplace(resource_pair.first,
                           makeResourcePtr<string>(requester_, endpoint, id_,
                                                   resource_pair.second));
        break;
      }
      case DataType::TIME:
      case DataType::UNSIGNED_INTEGER: {
        resources_.emplace(resource_pair.first,
                           makeResourcePtr<uint64_t>(requester_, endpoint, id_,
                                                     resource_pair.second));
        break;
      }
      case DataType::BOOLEAN:
      case DataType::NONE:
      default: {
        resources_.emplace(resource_pair.first,
                           makeResourcePtr<bool>(requester_, endpoint, id_,
                                                 resource_pair.second));
        break;
      }
      }
    }
  }
}

ResourceVariant ObjectInstance::getResource(uint32_t id) {
  auto it = resources_.find(id);
  if (it != resources_.end()) {
    return it->second;
  } else {
    throw ResourceDoesNotExist(ResourceID(id_, id));
  }
}

Resources ObjectInstance::getResources() { return resources_; }

} // namespace LwM2M