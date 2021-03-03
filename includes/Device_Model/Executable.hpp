#ifndef __LWM2M_EXECUTABLE_RESOURCE_HPP
#define __LWM2M_EXECUTABLE_RESOURCE_HPP

#include "Execute.hpp"
#include "Resource.hpp"

namespace LwM2M {
template <typename T>
class Executable : public Resource<T>, protected ResourceMetaInfo {
public:
  Executable(RequesterPtr requester, EndpointPtr endpoint,
             ObjectInstanceID parent, ResourceDescriptorPtr descriptor)
      : Resource<T>(),
        ResourceMetaInfo(requester, endpoint, parent, descriptor) {}

  ResourceDescriptorPtr getDescriptor() override { return descriptor_; }

  std::future<bool> execute(std::string arguments) override {
    auto message = std::make_shared<ExecuteRequest>(endpoint_, id_, arguments);

    return requester_->requestAction(message);
  }
};

template <typename T> using ExecutablePtr = std::shared_ptr<Executable<T>>;
} // namespace LwM2M
#endif //__LWM2M_EXECUTABLE_RESOURCE_HPP