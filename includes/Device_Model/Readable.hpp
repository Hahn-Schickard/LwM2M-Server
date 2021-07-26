#ifndef __LWM2M_READABLE_RESOURCE_HPP
#define __LWM2M_READABLE_RESOURCE_HPP

#include "Read.hpp"
#include "Resource.hpp"

namespace LwM2M {

template <typename T>
class Readable : public Resource<T>, protected ResourceMetaInfo {
  std::future<T> asyncDataRequest(ServerRequestPtr message) {
    return std::async(std::launch::async,
                      [](RequesterPtr requester, ServerRequestPtr msg) -> T {
                        auto result = requester->requestData(msg);
                        return result.get()->get<T>();
                      },
                      requester_, message);
  }

public:
  Readable(RequesterPtr requester, EndpointPtr endpoint, ObjectID parent,
           ResourceDescriptorPtr descriptor)
      : Resource<T>(),
        ResourceMetaInfo(requester, endpoint, parent, descriptor) {}

  ResourceDescriptorPtr getDescriptor() override { return descriptor_; }

  std::future<T> read() override {
    auto id = descriptor_->id_;
    auto target = ObjectID(parent_, parent_instance_, id);
    auto message = std::make_shared<ReadRequest>(endpoint_, target);
    return asyncDataRequest(message);
  }
};

template <typename T> using ReadablePtr = std::shared_ptr<Readable<T>>;
} // namespace LwM2M

#endif //__LWM2M_READABLE_RESOURCE_HPP