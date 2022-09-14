#include "Observe.hpp"

using namespace std;

namespace LwM2M {

ObserveRequest::ObserveRequest(EndpointPtr endpoint, // NOLINT
    ElementID target_id)
    : InformationReportingRequest(endpoint, // NOLINT
          MessageType::OBSERVE, make_shared<Payload>(target_id)) {}

string ObserveRequest::name() { return "ObserveRequest"; }
} // namespace LwM2M
