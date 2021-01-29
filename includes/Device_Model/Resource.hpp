#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include "Endpoint.hpp"
#include "ResourceDescriptor.hpp"
#include "ServerRequest_Read.hpp"
#include "ServerRequest_Write.hpp"

#include <future>
#include <memory>
#include <optional>
#include <variant>

namespace LwM2M {

using ResourceDescriptorPtr = std::shared_ptr<ResourceDescriptor>;
using DataVariant = std::variant<bool, int64_t, double, std::string, uint64_t,
                                 ObjectLink, std::vector<uint8_t>>;

struct UnsupportedMethod : public std::runtime_error {
  UnsupportedMethod(std::string const &message) : std::runtime_error(message) {}
};

struct UnsupportedParameter : public std::runtime_error {
  UnsupportedParameter(std::string const &message)
      : std::runtime_error(message) {}
};

template <typename T> class Resource {
  std::shared_ptr<Endpoint> endpoint_;
  uint32_t parent_id_;
  uint32_t parent_instance_id_;
  std::shared_ptr<RequestsManager> requests_manager_;
  ResourceDescriptorPtr descriptor_;

  T decode(ReturnType payload) {}

  ReturnType endcode(T value) {}

protected:
  std::future<T> asyncRead() {
    return std::async(std::launch::async, [&]() -> T {
      auto request = std::make_unique<ServerRequest_Read>(
          endpoint_->endpoint_address_, endpoint_->endpoint_port_, parent_id_,
          parent_instance_id_, descriptor_->id_);
      auto future = requests_manager_->request(std::move(request));
      future.wait();
      return decode(future.get());
    });
  }

  void asyncWrite(T value) {
    auto payload = endcode(value);
    auto request = std::make_unique<ServerRequest_Write>(
        endpoint_->endpoint_address_, endpoint_->endpoint_port_, parent_id_,
        parent_instance_id_, descriptor_->id_, std::nullopt, payload);
    requests_manager_->request(std::move(request));
  }

  void throwUnsupportedParameter(const char *exception_msg) {
    std::string error_msg =
        std::to_string(parent_id_) + ":" + std::to_string(parent_instance_id_) +
        ":" + std::to_string(descriptor_->id_) + " " + descriptor_->name_ +
        " was executed with an unsuported parameter: " + exception_msg;
    throw UnsupportedParameter(error_msg);
  }

public:
  Resource(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
           uint32_t parent_instance_id,
           std::shared_ptr<RequestsManager> requests_manager,
           ResourceDescriptorPtr descriptor)
      : endpoint_(endpoint), parent_id_(parent_id),
        parent_instance_id_(parent_instance_id),
        requests_manager_(requests_manager), descriptor_(descriptor) {}

  virtual T read() {
    throw UnsupportedMethod("This resource is not Readable!");
  }
  virtual std::future<T> read(size_t timeout) {
    throw UnsupportedMethod("This resource is not Readable!");
  }
  virtual void write(DataVariant value) {
    throw UnsupportedMethod("This resource is not Writable!");
  }
  virtual void write(T value) {
    throw UnsupportedMethod("This resource is not Writable!");
  }
  virtual void execute(T values...) {
    throw UnsupportedMethod("This resource is not Executable!");
  }

  ResourceDescriptorPtr getDescriptor() { return descriptor_; }
};

template <typename T> class Executable : public Resource<T> {
public:
  Executable(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
             uint32_t parent_instance_id,
             std::shared_ptr<RequestsManager> requests_manager,
             ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, requests_manager,
                    descriptor) {}

  void execute(T values...) override {}
};

template <typename T> class Readable : public Resource<T> {
public:
  Readable(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
           uint32_t parent_instance_id,
           std::shared_ptr<RequestsManager> requests_manager,
           ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, requests_manager,
                    descriptor) {}

  T read() override {
    auto value = this->asyncRead();
    return value.get();
  }

  std::future<T> read(size_t timeout) override { return this->asyncRead(); }
};

template <typename T> class Writable : public Resource<T> {
public:
  Writable(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
           uint32_t parent_instance_id,
           std::shared_ptr<RequestsManager> requests_manager,
           ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, requests_manager,
                    descriptor) {}

  void write(DataVariant variant_value) override {
    try {
      auto value = std::get<T>(variant_value);
      this->asyncWrite(value);
    } catch (std::bad_variant_access &ex) {
      this->throwUnsupportedParameter(ex.what());
    }
  }

  void write(T value) override { this->asyncWrite(value); }
};

template <typename T> class ReadAndWritable : public Resource<T> {
public:
  ReadAndWritable(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
                  uint32_t parent_instance_id,
                  std::shared_ptr<RequestsManager> requests_manager,
                  ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, requests_manager,
                    descriptor) {}

  T read() override {
    auto value = this->asyncRead();
    return value.get();
  }

  std::future<T> read(size_t timeout) override { return this->asyncRead(); }

  void write(DataVariant variant_value) override {
    try {
      auto value = std::get<T>(variant_value);
      this->asyncWrite(value);
    } catch (std::bad_variant_access &ex) {
      this->throwUnsupportedParameter(ex.what());
    }
  }

  void write(T value) override { this->asyncWrite(value); }
};
} // namespace LwM2M

#endif //__LWM2M_MODEL_RESOURCE_HPP