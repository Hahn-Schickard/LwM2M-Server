#include "Readable.hpp"
#include "Read.hpp"

using namespace std;

namespace LwM2M {

future<DataVariant> Readable::asyncDataRequest(
    DeviceManagementRequestPtr message) {
  return async(
      launch::async,
      [&](RequesterPtr requester,
          DeviceManagementRequestPtr msg) -> DataVariant {
        auto result = requester->requestData(msg);
        return result.get()->get(getDataType(id_));
      },
      requester_, message);
}

Readable::Readable(Observable::ExceptionHandler handler, RequesterPtr requester,
    EndpointPtr endpoint, ElementID id, DataType data_type)
    : Observable(handler, requester, endpoint, id, data_type) {}

future<DataVariant> Readable::read() {
  auto message = make_shared<ReadRequest>(endpoint_, id_);
  return asyncDataRequest(message);
}
} // namespace LwM2M