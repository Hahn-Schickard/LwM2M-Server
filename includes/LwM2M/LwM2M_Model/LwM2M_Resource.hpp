#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include "LwM2M_Endpoint.hpp"
#include "LwM2M_ResourceDescriptor.hpp"
#include "Read_Request.hpp"
#include "Response_Handler.hpp"

#include <future>
#include <memory>
#include <optional>

namespace LwM2M {

using ResourceDescriptorPtr = std::shared_ptr<ResourceDescriptor>;

struct UnsupportedMethod : public std::runtime_error {
  UnsupportedMethod(std::string const &message) : std::runtime_error(message) {}
};

template <typename T> class Resource {
protected:
  std::shared_ptr<Endpoint> endpoint_;
  uint32_t parent_id_;
  uint32_t parent_instance_id_;
  std::shared_ptr<ResponseHandler> response_handler_;
  ResourceDescriptorPtr descriptor_;

public:
  Resource(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
           uint32_t parent_instance_id,
           std::shared_ptr<ResponseHandler> response_handler,
           ResourceDescriptorPtr descriptor)
      : endpoint_(endpoint), parent_id_(parent_id),
        parent_instance_id_(parent_instance_id),
        response_handler_(response_handler), descriptor_(descriptor) {}

  virtual T read() {
    throw UnsupportedMethod("This resource is not Readable!");
  }
  virtual std::future<T> read(size_t timeout) {
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
  Executable(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
             uint32_t parent_instance_id,
             std::shared_ptr<ResponseHandler> response_handler,
             ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, response_handler,
                    descriptor) {}

  std::future<T> execute(T values...) override {}
};

template <typename T> class Readable : public Resource<T> {
public:
  Readable(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
           uint32_t parent_instance_id,
           std::shared_ptr<ResponseHandler> response_handler,
           ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, response_handler,
                    descriptor) {}

  T read() override {
    auto request = std::make_unique<Read_Request>(
        this->endpoint_->endpoint_address_, this->endpoint_->endpoint_port_,
        this->parent_id_, this->parent_instance_id_, this->descriptor_->id_);
    ResponseFuture future =
        this->response_handler_->generateRequest(std::move(request));
  }
  std::future<T> read(size_t timeout) override {}
};

template <typename T> class Writable : public Resource<T> {
public:
  Writable(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
           uint32_t parent_instance_id,
           std::shared_ptr<ResponseHandler> response_handler,
           ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, response_handler,
                    descriptor) {}

  std::future<T> write(T value) override {}
};

template <typename T> class ReadAndWritable : public Resource<T> {
public:
  ReadAndWritable(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
                  uint32_t parent_instance_id,
                  std::shared_ptr<ResponseHandler> response_handler,
                  ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, response_handler,
                    descriptor) {}

  T read() override {}
  std::future<T> read(size_t timeout) override {}
  std::future<T> write(T value) override {}
};

} // namespace LwM2M

#endif //__LWM2M_MODEL_RESOURCE_HPP