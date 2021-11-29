#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include "CallableEntity.hpp"
#include "ResourceDescriptor.hpp"

#include <future>

namespace LwM2M {

struct UnsupportedMethod : public std::runtime_error {
  UnsupportedMethod(std::string const &message) : std::runtime_error(message) {}
};

struct UnsupportedParameter : public std::runtime_error {
  UnsupportedParameter(std::string const &message)
      : std::runtime_error(message) {}
};

template <typename T> class Resource {
  ResourceDescriptorPtr descriptor_;

public:
  Resource() = default;

  Resource(ResourceDescriptorPtr descriptor) : descriptor_(descriptor) {}

  virtual ~Resource() = default;

  ResourceDescriptorPtr getDescriptor() { return descriptor_; }

  virtual std::future<T> read() {
    throw UnsupportedMethod("This resource is not Readable!");
  }

  virtual std::future<bool> write(DataVariant /*value*/) {
    throw UnsupportedMethod("This resource is not Writable!");
  }

  virtual std::future<bool> write(T /*value*/) {
    throw UnsupportedMethod("This resource is not Writable!");
  }

  virtual std::future<bool> execute(std::string /*arguments*/ = std::string()) {
    throw UnsupportedMethod("This resource is not Executable!");
  }
};

template <typename T> using ResourcePtr = std::shared_ptr<Resource<T>>;
} // namespace LwM2M

#endif //__LWM2M_MODEL_RESOURCE_HPP