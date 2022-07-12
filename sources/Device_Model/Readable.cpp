#include "Readable.hpp"
#include "Read.hpp"

using namespace std;

namespace LwM2M {

future<DataVariant> Readable::asyncDataRequest(
    DeviceManagementRequestPtr message) {
  return async(
      launch::async,
      [](RequesterPtr requester,
          DeviceManagementRequestPtr msg) -> DataVariant {
        auto result = requester->requestData(msg);
        return result.get()->get<DataVariant>();
      },
      requester_, message);
}

Readable::Readable(ResourceDescriptorPtr descriptor,
    Observable::ExceptionHandler handler, RequesterPtr requester,
    EndpointPtr endpoint, ElementID id)
    : Observable(handler, requester, endpoint, id, descriptor->data_type_) {}

future<DataVariant> Readable::read() {
  auto message = make_shared<ReadRequest>(endpoint_, id_);
  return asyncDataRequest(message);
}
} // namespace LwM2M