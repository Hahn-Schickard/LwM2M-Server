#include "Writable.hpp"
#include "Write.hpp"

using namespace std;

namespace LwM2M {
Writable::Writable(
    RequesterInterfaceFacadePtr requester, EndpointPtr endpoint, ElementID id)
    : CallableEntity(requester, endpoint, id) {}

future<bool> Writable::write(DataVariant data) {
  auto payload = make_shared<DataFormat>(data);
  auto message = make_shared<WriteRequest>(endpoint_, id_, payload);

  return requester_->requestAction(message);
}
} // namespace LwM2M