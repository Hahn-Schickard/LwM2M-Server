#ifndef __LWM2M_READABLE_RESOURCE_HPP
#define __LWM2M_READABLE_RESOURCE_HPP

#include "Observable.hpp"
#include "Read.hpp"
#include "Resource.hpp"

namespace LwM2M {

template <typename T> class Readable : public Resource<T>, public Observable {
  std::future<T> asyncDataRequest(DeviceManagementRequestPtr message) {
    return std::async(
        std::launch::async,
        [](RequesterPtr requester, DeviceManagementRequestPtr msg) -> T {
          auto result = requester->requestData(msg);
          return result.get()->get<T>();
        },
        requester_, message);
  }

public:
  Readable(ResourceDescriptorPtr descriptor,
           Observable::ExceptionHandler handler, RequesterPtr requester,
           EndpointPtr endpoint, ElementID id)
      : Resource<T>(descriptor), Observable(handler, requester, endpoint, id) {}

  std::future<T> read() override {
    auto message = std::make_shared<ReadRequest>(endpoint_, id_);
    return asyncDataRequest(message);
  }
};

template <typename T> using ReadablePtr = std::shared_ptr<Readable<T>>;
} // namespace LwM2M

#endif //__LWM2M_READABLE_RESOURCE_HPP