#include "CancelObservation.hpp"

using namespace std;

namespace LwM2M {

CancelObservationRequest::CancelObservationRequest(EndpointPtr endpoint,
                                                   ElementID target_id)
    : InformationReportingRequest(endpoint, MessageType::CANCEL_OBSERVATION,
                                  make_shared<Payload>(target_id)) {}

string CancelObservationRequest::name() { return "CancelObservationRequest"; }
} // namespace LwM2M
