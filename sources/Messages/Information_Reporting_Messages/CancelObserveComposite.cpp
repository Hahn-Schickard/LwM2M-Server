#include "CancelObserveComposite.hpp"

using namespace std;

namespace LwM2M {
CancelObserveCompositeRequest::CancelObserveCompositeRequest(
    EndpointPtr endpoint, ObjectIDs target_ids)
    : ServerRequest(endpoint, MessageType::CANCEL_OBSERVATION_COMPOSITE,
                    InterfaceType::INFORMATION_REPORTING,
                    make_shared<Payload>(target_ids)) {}

string CancelObserveCompositeRequest::name() {
  return "CancelObserveCompositeRequest";
}
} // namespace LwM2M
