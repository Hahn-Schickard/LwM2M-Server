#include "ObjectInstance.hpp"

using namespace std;

namespace LwM2M {

template <typename T>
shared_ptr<Resource<T>>
makeResourcePtr(shared_ptr<Endpoint> endpoint, uint32_t parent_id,
                uint32_t instance_id,
                shared_ptr<RequestsManager> requests_manager,
                ResourceDescriptorPtr descriptor) {
  switch (descriptor->operations_) {
  case OperationsType::READ: {
    return make_shared<Readable<T>>(endpoint, parent_id, instance_id,
                                    requests_manager, descriptor);
  }
  case OperationsType::WRITE: {
    return make_shared<Writable<T>>(endpoint, parent_id, instance_id,
                                    requests_manager, descriptor);
  }
  case OperationsType::READ_AND_WRITE: {
    return make_shared<ReadAndWritable<T>>(endpoint, parent_id, instance_id,
                                           requests_manager, descriptor);
  }
  case OperationsType::EXECUTE: {
    return make_shared<Executable<T>>(endpoint, parent_id, instance_id,
                                      requests_manager, descriptor);
  }
  case OperationsType::NO_OPERATION:
  default: {
    return make_shared<Resource<T>>(endpoint, parent_id, instance_id,
                                    requests_manager, descriptor);
  }
  }
}

ObjectInstance::ObjectInstance(
    std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
    uint32_t instance_id, std::shared_ptr<RequestsManager> requests_manager,
    std::unordered_map<uint32_t, std::shared_ptr<ResourceDescriptor>>
        resource_descriptors)
    : endpoint_(endpoint), parent_id_(parent_id), instance_id_(instance_id),
      requests_manager_(requests_manager) {
  for (auto resource_pair : resource_descriptors) {
    switch (resource_pair.second->data_type_) {
    case DataType::FLOAT: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<double>(endpoint, parent_id_, instance_id_,
                                  requests_manager_, resource_pair.second));
      break;
    }
    case DataType::SIGNED_INTEGER: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<int64_t>(endpoint, parent_id_, instance_id_,
                                   requests_manager_, resource_pair.second));
      break;
    }
    case DataType::OBJECT_LINK: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<ObjectLink>(endpoint, parent_id_, instance_id_,
                                      requests_manager_, resource_pair.second));
      break;
    }
    case DataType::OPAQUE: {
      resources_.emplace(resource_pair.first,
                         makeResourcePtr<vector<uint8_t>>(
                             endpoint, parent_id_, instance_id_,
                             requests_manager_, resource_pair.second));
      break;
    }
    case DataType::STRING: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<string>(endpoint, parent_id_, instance_id_,
                                  requests_manager_, resource_pair.second));
      break;
    }
    case DataType::TIME: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<time_t>(endpoint, parent_id_, instance_id_,
                                  requests_manager_, resource_pair.second));
      break;
    }
    case DataType::BOOLEAN:
    case DataType::NONE:
    default: {
      resources_.emplace(resource_pair.first,
                         makeResourcePtr<bool>(endpoint, parent_id_,
                                               instance_id_, requests_manager_,
                                               resource_pair.second));
      break;
    }
    }
  }
}

ResourceVariant ObjectInstance::getResource(uint32_t id) {
  auto it = resources_.find(id);
  if (it != resources_.end()) {
    return it->second;
  } else {
    return ResourceVariant();
  }
}

Resources ObjectInstance::getResources() { return resources_; }

} // namespace LwM2M