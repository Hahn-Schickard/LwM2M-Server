#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include "DataFormat.hpp"
#include "Endpoint.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {

using ResourceDescriptorPtr = std::shared_ptr<ResourceDescriptor>;

struct UnsupportedMethod : public std::runtime_error {
  UnsupportedMethod(std::string const &message) : std::runtime_error(message) {}
};

struct UnsupportedParameter : public std::runtime_error {
  UnsupportedParameter(std::string const &message)
      : std::runtime_error(message) {}
};

template <typename T> class Resource {
  EndpointPtr endpoint_;
  uint32_t parent_id_;
  uint32_t parent_instance_id_;
  ResourceDescriptorPtr descriptor_;

public:
  Resource(EndpointPtr endpoint, uint32_t parent_id,
           uint32_t parent_instance_id, ResourceDescriptorPtr descriptor)
      : endpoint_(endpoint), parent_id_(parent_id),
        parent_instance_id_(parent_instance_id), descriptor_(descriptor) {}

  ResourceDescriptorPtr getDescriptor() { return descriptor_; }
};

template <typename T> class Executable : public Resource<T> {
public:
  Executable(EndpointPtr endpoint, uint32_t parent_id,
             uint32_t parent_instance_id, ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, descriptor) {}
};

template <typename T> class Readable : public Resource<T> {
public:
  Readable(EndpointPtr endpoint, uint32_t parent_id,
           uint32_t parent_instance_id, ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, descriptor) {}
};

template <typename T> class Writable : public Resource<T> {
public:
  Writable(EndpointPtr endpoint, uint32_t parent_id,
           uint32_t parent_instance_id,

           ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, descriptor) {}
};

template <typename T> class ReadAndWritable : public Resource<T> {
public:
  ReadAndWritable(EndpointPtr endpoint, uint32_t parent_id,
                  uint32_t parent_instance_id, ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, descriptor) {}
};
} // namespace LwM2M

#endif //__LWM2M_MODEL_RESOURCE_HPP