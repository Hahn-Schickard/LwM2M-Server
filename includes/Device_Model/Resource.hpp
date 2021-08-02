#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include "ElementID.hpp"
#include "Endpoint.hpp"
#include "Requester.hpp"
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

struct ResourceMetaInfo {
  RequesterPtr requester_;
  EndpointPtr endpoint_;
  ResourceDescriptorPtr descriptor_;
  ElementID id_;

  ResourceMetaInfo(RequesterPtr requester, EndpointPtr endpoint, ElementID id,
                   ResourceDescriptorPtr descriptor)
      : requester_(requester), endpoint_(endpoint), descriptor_(descriptor),
        id_(id) {
    if (!descriptor_) {
      throw std::invalid_argument("Resource descriptor can not be nullptr");
    }
  }
}; // namespace LwM2M

template <typename T> class Resource {
public:
  virtual ~Resource() = default;

  virtual ResourceDescriptorPtr getDescriptor() {
    return ResourceDescriptorPtr();
  }
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