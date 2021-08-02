#include "CancelObservation.hpp"

using namespace std;

namespace LwM2M {

CancelObservationRequest::CancelObservationRequest(EndpointPtr endpoint,
                                                   ElementID target_id)
    : ServerRequest(endpoint, MessageType::CANCEL_OBSERVATION,
                    InterfaceType::INFORMATION_REPORTING,
                    make_shared<Payload>(target_id)) {}

string CancelObservationRequest::name() { return "CancelObservationRequest"; }
} // namespace LwM2M
