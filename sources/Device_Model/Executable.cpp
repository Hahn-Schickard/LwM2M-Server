#include "Executable.hpp"
#include "Execute.hpp"

using namespace std;

namespace LwM2M {

Executable::Executable(
    RequesterPtr requester, EndpointPtr endpoint, ElementID id)
    : CallableEntity(requester, endpoint, id) {}

future<bool> Executable::execute(string arguments) {
  auto message = make_shared<ExecuteRequest>(endpoint_, id_, arguments);

  return requester_->requestAction(message);
}

} // namespace LwM2M