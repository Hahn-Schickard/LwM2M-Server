#ifndef __LWM2M_READ_AND_WRITABLE_RESOURCE_HPP
#define __LWM2M_READ_AND_WRITABLE_RESOURCE_HPP

#include "Observable.hpp"
#include "Read.hpp"
#include "Resource.hpp"
#include "Write.hpp"

namespace LwM2M {
template <typename T>
class ReadAndWritable : public Resource<T>, public Observable {
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
  ReadAndWritable(ResourceDescriptorPtr descriptor,
                  Observable::ExceptionHandler handler, RequesterPtr requester,
                  EndpointPtr endpoint, ElementID id)
      : Resource<T>(descriptor), Observable(handler, requester, endpoint, id) {}

  std::future<T> read() override {
    auto message = std::make_shared<ReadRequest>(endpoint_, id_);
    return asyncDataRequest(message);
  }

  std::future<bool> write(DataVariant data) override {
    auto payload = std::make_shared<DataFormat>(data);
    auto message = std::make_shared<WriteRequest>(endpoint_, id_, payload);

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