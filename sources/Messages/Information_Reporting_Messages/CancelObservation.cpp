#include "CancelObservation.hpp"

using namespace std;

namespace LwM2M {

CancelObservationRequest::CancelObservationRequest(
    EndpointPtr endpoint, // NOLINT
    ElementID target_id)
    : InformationReportingRequest(endpoint, // NOLINT
          MessageType::CANCEL_OBSERVATION, make_shared<Payload>(target_id)) {}

string CancelObservationRequest::name() const {
  return "CancelObservationRequest";
}
} // namespace LwM2M
