#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include "LwM2M_Endpoint.hpp"
#include "LwM2M_ResourceDescriptor.hpp"

#include <future>
#include <memory>
#include <optional>

namespace LwM2M {

using ResourceDescriptorPtr = std::shared_ptr<ResourceDescriptor>;

struct UnsupportedMethod : public std::runtime_error {
  UnsupportedMethod(std::string const &message) : std::runtime_error(message) {}
};

template <typename T> class Resource {
  std::shared_ptr<Endpoint> endpoint_;
  ResourceDescriptorPtr descriptor_;

public:
  Resource(std::shared_ptr<Endpoint> endpoint, ResourceDescriptorPtr descriptor)
      : endpoint_(endpoint), descriptor_(descriptor) {}

  virtual std::future<T> read() {
    throw UnsupportedMethod("This resource is not Readable!");
  }
  virtual std::future<T> write(T value) {
    throw UnsupportedMethod("This resource is not Writable!");
  }
  virtual std::future<T> execute(T values...) {
    throw UnsupportedMethod("This resource is not Executable!");
  }

  ResourceDescriptorPtr getDescriptor() { return descriptor_; }
};

template <typename T> class Executable : public Resource<T> {
public:
  Executable(std::shared_ptr<Endpoint> endpoint,
             ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, descriptor) {}

  std::future<T> execute(T values...) override {}
};

template <typename T> class Readable : public Resource<T> {
public:
  Readable(std::shared_ptr<Endpoint> endpoint, ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, descriptor) {}

  std::future<T> read() override {}
};

template <typename T> class Writable : public Resource<T> {
public:
  Writable(std::shared_ptr<Endpoint> endpoint, ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, descriptor) {}

  std::future<T> write(T value) override {}
};

template <typename T> class ReadAndWritable : public Resource<T> {
public:
  ReadAndWritable(std::shared_ptr<Endpoint> endpoint,
                  ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, descriptor) {}

  std::future<T> read() override {}
  std::future<T> write(T value) override {}
};

} // namespace LwM2M

#endif //__LWM2M_MODEL_RESOURCE_HPP