#ifndef __LWM2M_MODEL_RESOURCE_HPP
#define __LWM2M_MODEL_RESOURCE_HPP

#include "LwM2M_Endpoint.hpp"
#include "LwM2M_ResourceDescriptor.hpp"
#include "LwM2M_TLV.hpp"
#include "Message_Encoder.hpp"
#include "Read_Request.hpp"

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
  uint32_t parent_id_;
  uint32_t parent_instance_id_;
  std::shared_ptr<MessageEncoder> encoder_;
  ResourceDescriptorPtr descriptor_;

  T decode(ReturnType payload) {
    std::shared_ptr<TLV_Pack> tlv_payload =
        std::static_pointer_cast<TLV_Pack>(payload);
    return tlv_payload->getValue<T>(descriptor_->id_);
  }

protected:
  std::future<T> asyncRead() {
    return std::async(std::launch::async, [&]() -> T {
      auto request = std::make_unique<Read_Request>(
          endpoint_->endpoint_address_, endpoint_->endpoint_port_, parent_id_,
          parent_instance_id_, descriptor_->id_);
      auto future = encoder_->encode(std::move(request));
      future.wait();
      return decode(future.get());
    });
  }

public:
  Resource(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
           uint32_t parent_instance_id, std::shared_ptr<MessageEncoder> encoder,
           ResourceDescriptorPtr descriptor)
      : endpoint_(endpoint), parent_id_(parent_id),
        parent_instance_id_(parent_instance_id), encoder_(encoder),
        descriptor_(descriptor) {}

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
             std::shared_ptr<MessageEncoder> encoder,
             ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, encoder,
                    descriptor) {}

  std::future<T> execute(T values...) override {}
};

template <typename T> class Readable : public Resource<T> {
public:
  Readable(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
           uint32_t parent_instance_id, std::shared_ptr<MessageEncoder> encoder,
           ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, encoder,
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
           uint32_t parent_instance_id, std::shared_ptr<MessageEncoder> encoder,
           ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, encoder,
                    descriptor) {}

  std::future<T> write(T value) override {}
};

template <typename T> class ReadAndWritable : public Resource<T> {
public:
  ReadAndWritable(std::shared_ptr<Endpoint> endpoint, uint32_t parent_id,
                  uint32_t parent_instance_id,
                  std::shared_ptr<MessageEncoder> encoder,
                  ResourceDescriptorPtr descriptor)
      : Resource<T>(endpoint, parent_id, parent_instance_id, encoder,
                    descriptor) {}

  T read() override {}
  std::future<T> read(size_t timeout) override {}
  std::future<T> write(T value) override {}
};

} // namespace LwM2M

#endif //__LWM2M_MODEL_RESOURCE_HPP