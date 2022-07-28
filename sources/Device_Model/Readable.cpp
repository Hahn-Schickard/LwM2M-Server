#include "Readable.hpp"
#include "Read.hpp"

#include <future>

using namespace std;

namespace LwM2M {

Readable::Readable(Observable::ExceptionHandler handler,
    ObservableInterfacePtr observe_requester,
    ReadableInterfacePtr read_requester, EndpointPtr endpoint, ElementID id,
    DataType data_type)
    : Observable(handler, observe_requester, endpoint, id, data_type),
      requester_(read_requester) {}

Readable::Result Readable::read() {
  auto message = make_shared<ReadRequest>(endpoint_, id_);

  auto future_result = async(
      launch::async,
      [&](ReadableInterfacePtr requester,
          DeviceManagementRequestPtr msg) -> DataVariant {
        auto result = requester->requestData(msg);
        return result.get()->get(getDataType(id_));
      },
      requester_, message);
  return requester_->issueCancelable(message, move(future_result));
}
} // namespace LwM2M