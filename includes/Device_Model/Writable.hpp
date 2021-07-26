#ifndef __LWM2M_WRITABLE_RESOURCE_HPP
#define __LWM2M_WRITABLE_RESOURCE_HPP

#include "Resource.hpp"
#include "Write.hpp"

namespace LwM2M {

template <typename T>
class Writable : public Resource<T>, protected ResourceMetaInfo {
public:
  Writable(RequesterPtr requester, EndpointPtr endpoint, ObjectID parent,
           ResourceDescriptorPtr descriptor)
      : Resource<T>(),
        ResourceMetaInfo(requester, endpoint, parent, descriptor) {}

  ResourceDescriptorPtr getDescriptor() override { return descriptor_; }

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

template <typename T> using WritablePtr = std::shared_ptr<Writable<T>>;

} // namespace LwM2M

#endif //__LWM2M_WRITABLE_RESOURCE_HPP