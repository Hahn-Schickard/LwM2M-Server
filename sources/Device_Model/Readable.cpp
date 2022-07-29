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

  auto result_future = requester_->requestData(message);
  auto resolved_future = async(
      launch::async,
      [&](std::future<DataFormatPtr>&& result) -> DataVariant {
        return result.get()->get(getDataType(id_));
      },
      move(result_future));
  return requester_->issueCancelable(message, move(resolved_future));
}
} // namespace LwM2M