#include "Writable.hpp"
#include "Write.hpp"

using namespace std;

namespace LwM2M {
Writable::Writable(const ExecutableInterfacePtr& requester,
    const EndpointPtr& endpoint, const ElementID& id)
    : ElementAddress(endpoint, id), requester_(requester) {}

Writable::Result Writable::write(const DataVariant& data) {
  auto payload = make_shared<DataFormat>(data);
  auto message = make_shared<WriteRequest>(endpoint_, id_, payload);

  auto result_future = requester_->requestAction(message);
  return requester_->issueCancelable(message, move(result_future));
}
} // namespace LwM2M