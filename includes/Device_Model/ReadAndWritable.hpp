#ifndef __LWM2M_READ_AND_WRITABLE_RESOURCE_HPP
#define __LWM2M_READ_AND_WRITABLE_RESOURCE_HPP

#include "Read.hpp"
#include "Resource.hpp"
#include "Write.hpp"

namespace LwM2M {
template <typename T>
class ReadAndWritable : public Resource<T>, protected ResourceMetaInfo {
  std::future<T> asyncDataRequest(ServerRequestPtr message) {
    return std::async(std::launch::async,
                      [](RequesterPtr requester, ServerRequestPtr msg) -> T {
                        auto result = requester->requestData(msg);
                        return result.get()->get<T>();
                      },
                      requester_, message);
  }

public:
  ReadAndWritable(RequesterPtr requester, EndpointPtr endpoint, ObjectID parent,
                  ResourceDescriptorPtr descriptor)
      : Resource<T>(),
        ResourceMetaInfo(requester, endpoint, parent, descriptor) {}

  ResourceDescriptorPtr getDescriptor() override { return descriptor_; }

  std::future<T> read() override {
    auto target = ObjectID(parent_, parent_instance_, descriptor_->id_);
    auto message = std::make_shared<ReadRequest>(endpoint_, target);
    return asyncDataRequest(message);
  }

  std::future<bool> write(DataVariant data) override {
    auto target = ObjectID(parent_, parent_instance_, descriptor_->id_);
    auto payload = std::make_shared<DataFormat>(data);
    auto message = std::make_shared<WriteRequest>(endpoint_, target, payload);

    return requester_->requestAction(message);
  }

  std::future<bool> write(T value) override {
    return write(DataVariant(value));
  }
};

template <typename T>
using ReadAndWritablePtr = std::shared_ptr<ReadAndWritable<T>>;
} // namespace LwM2M

#endif //__LWM2M_READ_AND_WRITABLE_RESOURCE_HPP