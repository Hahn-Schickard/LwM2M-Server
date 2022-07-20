#include "Executable.hpp"
#include "Execute.hpp"

using namespace std;

namespace LwM2M {

Executable::Executable(
    ExecutableInterfacePtr requester, EndpointPtr endpoint, ElementID id)
    : CallableEntity(endpoint, id), requester_(requester) {}

future<bool> Executable::execute(string arguments) {
  auto message = make_shared<ExecuteRequest>(endpoint_, id_, arguments);

  return requester_->requestAction(message);
}

} // namespace LwM2M