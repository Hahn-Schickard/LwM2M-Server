#include "Executable.hpp"
#include "Execute.hpp"

using namespace std;

namespace LwM2M {

Executable::Executable(ExecutableInterfacePtr requester, // NOLINT
    EndpointPtr endpoint, // NOLINT
    ElementID id)
    : ElementAddress(endpoint, id), requester_(requester) {} // NOLINT

Executable::Result Executable::execute(string arguments) {
  auto message = make_shared<ExecuteRequest>(endpoint_, id_, arguments);

  auto result_future = requester_->requestAction(message);
  return requester_->issueCancelable(message, move(result_future));
}

} // namespace LwM2M