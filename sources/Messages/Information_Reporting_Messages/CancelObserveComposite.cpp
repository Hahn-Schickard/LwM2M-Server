#include "CancelObserveComposite.hpp"

using namespace std;

namespace LwM2M {
CancelObserveCompositeRequest::CancelObserveCompositeRequest(
    EndpointPtr endpoint, // NOLINT
    ElementIDs target_ids) // NOLINT
    : InformationReportingRequest(endpoint, // NOLINT
          MessageType::CANCEL_OBSERVATION_COMPOSITE,
          make_shared<Payload>(target_ids)) {}

string CancelObserveCompositeRequest::name() {
  return "CancelObserveCompositeRequest";
}
} // namespace LwM2M
