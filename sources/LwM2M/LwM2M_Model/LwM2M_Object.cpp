#include "LwM2M_Object.hpp"

using namespace std;

namespace LwM2M {

template <typename T>
shared_ptr<Resource<T>> makeResourcePtr(shared_ptr<Endpoint> endpoint,
                                        ResourceDescriptorPtr descriptor) {
  switch (descriptor->operations_) {
  case OperationsType::READ: {
    return make_shared<Readable<T>>(endpoint, descriptor);
  }
  case OperationsType::WRITE: {
    return make_shared<Writable<T>>(endpoint, descriptor);
  }
  case OperationsType::READ_AND_WRITE: {
    return make_shared<ReadAndWritable<T>>(endpoint, descriptor);
  }
  case OperationsType::EXECUTE: {
    return make_shared<Executable<T>>(endpoint, descriptor);
  }
  case OperationsType::NO_OPERATION:
  default: { return make_shared<Resource<T>>(endpoint, descriptor); }
  }
}

Object::Object(std::shared_ptr<Endpoint> endpoint,
               ObjectDescriptorPtr descriptor)
    : endpoint_(endpoint), descriptor_(descriptor) {
  for (auto resource_pair : descriptor_->resources_) {
    switch (resource_pair.second->data_type_) {
    case DataType::FLOAT: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<double>(endpoint, resource_pair.second));
      break;
    }
    case DataType::INTEGER: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<int64_t>(endpoint, resource_pair.second));
      break;
    }
    case DataType::OBJECT_LINK: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<ObjectLink>(endpoint, resource_pair.second));
      break;
    }
    case DataType::OPAQUE: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<vector<uint8_t>>(endpoint, resource_pair.second));
      break;
    }
    case DataType::STRING: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<string>(endpoint, resource_pair.second));
      break;
    }
    case DataType::TIME: {
      resources_.emplace(
          resource_pair.first,
          makeResourcePtr<time_t>(endpoint, resource_pair.second));
      break;
    }
    case DataType::BOOLEAN:
    case DataType::NONE:
    default: {
      resources_.emplace(resource_pair.first,
                         makeResourcePtr<bool>(endpoint, resource_pair.second));
      break;
    }
    }
  }
}

ObjectDescriptorPtr Object::getDescriptor() { return descriptor_; }
} // namespace LwM2M
