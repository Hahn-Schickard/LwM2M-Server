#include "ObserveComposite.hpp"

using namespace std;

namespace LwM2M {
ObserveCompositeRequest::ObserveCompositeRequest(EndpointPtr endpoint, // NOLINT
    ElementIDs target_ids) // NOLINT
    : InformationReportingRequest(endpoint, // NOLINT
          MessageType::Observe_Composite, make_shared<Payload>(target_ids)) {}

string ObserveCompositeRequest::name() const {
  return "ObserveCompositeRequest";
}
} // namespace LwM2M
