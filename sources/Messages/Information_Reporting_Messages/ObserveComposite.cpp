#include "ObserveComposite.hpp"

using namespace std;

namespace LwM2M {
ObserveCompositeRequest::ObserveCompositeRequest(EndpointPtr endpoint, // NOLINT
    ElementIDs target_ids)
    : InformationReportingRequest(endpoint, // NOLINT
          MessageType::OBSERVE_COMPOSITE, make_shared<Payload>(target_ids)) {}

string ObserveCompositeRequest::name() { return "ObserveCompositeRequest"; }
} // namespace LwM2M
