#include "ObjectInstance.hpp"

#include "Executable.hpp"
#include "ReadAndWritable.hpp"
#include "Readable.hpp"
#include "Writable.hpp"

using namespace std;

namespace LwM2M {

template <typename T>
shared_ptr<Resource<T>> makeResourcePtr(Observable::ExceptionHandler handler,
                                        RequesterPtr requester,
                                        EndpointPtr endpoint, ElementID id,
                                        ResourceDescriptorPtr descriptor) {
  switch (descriptor->operations_) {
  case OperationsType::READ: {
    return make_shared<Readable<T>>(descriptor, handler, requester, endpoint,
                                    id);
  }
  case OperationsType::WRITE: {
    return make_shared<Writable<T>>(descriptor, requester, endpoint, id);
  }
  case OperationsType::READ_AND_WRITE: {
    return make_shared<ReadAndWritable<T>>(descriptor, handler, requester,
                                           endpoint, id);
  }
  case OperationsType::EXECUTE: {
    return make_shared<Executable<T>>(descriptor, requester, endpoint, id);
  }
  case OperationsType::NO_OPERATION:
  default: { return make_shared<Resource<T>>(); }
  }
}

ObjectInstance::ObjectInstance(
    Observable::ExceptionHandler handler, RequesterPtr requester,
    EndpointPtr endpoint, ElementID id,
    unordered_map<uint32_t, ResourceDescriptorPtr> resource_descriptors)
    : requester_(requester), endpoint_(endpoint), id_(id) {
  for (auto resource_pair : resource_descriptors) {
    switch (resource_pair.second->data_type_) {
    case DataType::FLOAT: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<double>(handler, requester_, endpoint,
                                  ElementID(id_.getObjectID(),
                                            id.getObjectInstanceID(),
                                            resource_pair.second->id_),
                                  resource_pair.second));
      break;
    }
    case DataType::SIGNED_INTEGER: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<int64_t>(handler, requester_, endpoint,
                                   ElementID(id_.getObjectID(),
                                             id.getObjectInstanceID(),
                                             resource_pair.second->id_),
                                   resource_pair.second));
      break;
    }
    case DataType::OBJECT_LINK: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<ObjectLink>(handler, requester_, endpoint,
                                      ElementID(id_.getObjectID(),
                                                id.getObjectInstanceID(),
                                                resource_pair.second->id_),
                                      resource_pair.second));
      break;
    }
    case DataType::OPAQUE: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<vector<uint8_t>>(handler, requester_, endpoint,
                                           ElementID(id_.getObjectID(),
                                                     id.getObjectInstanceID(),
                                                     resource_pair.second->id_),
                                           resource_pair.second));
      break;
    }
    case DataType::STRING: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<string>(handler, requester_, endpoint,
                                  ElementID(id_.getObjectID(),
                                            id.getObjectInstanceID(),
                                            resource_pair.second->id_),
                                  resource_pair.second));
      break;
    }
    case DataType::TIME: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<TimeStamp>(handler, requester_, endpoint,
                                     ElementID(id_.getObjectID(),
                                               id.getObjectInstanceID(),
                                               resource_pair.second->id_),
                                     resource_pair.second));
      break;
    }
    case DataType::UNSIGNED_INTEGER: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<uint64_t>(handler, requester_, endpoint,
                                    ElementID(id_.getObjectID(),
                                              id.getObjectInstanceID(),
                                              resource_pair.second->id_),
                                    resource_pair.second));
      break;
    }
    case DataType::BOOLEAN:
    case DataType::NONE:
    default: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<bool>(handler, requester_, endpoint,
                                ElementID(id_.getObjectID(),
                                          id.getObjectInstanceID(),
                                          resource_pair.second->id_),
                                resource_pair.second));
      break;
    }
    }
  }
}

ElementID ObjectInstance::getId() { return id_; }

ResourceVariant ObjectInstance::getResource(ElementID id) {
  auto it = resources_.find(id.getResourceID());
  if (it != resources_.end()) {
    return it->second;
  } else {
    throw ResourceDoesNotExist(id);
  }
}

Resources ObjectInstance::getResources() { return resources_; }

} // namespace LwM2M