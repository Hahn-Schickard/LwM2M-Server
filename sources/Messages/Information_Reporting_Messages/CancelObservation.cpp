#include "CancelObservation.hpp"

using namespace std;

namespace LwM2M {

CancelObservationRequest::CancelObservationRequest(EndpointPtr endpoint,
                                                   ElmentIdVariant target_id)
    : ServerRequest(endpoint, MessageType::CANCEL_OBSERVATION,
                    InterfaceType::INFORMATION_REPORTING),
      target_id_(target_id) {}

string CancelObservationRequest::name() { return "CancelObservationRequest"; }

CancelObservationResponse::CancelObservationResponse(EndpointPtr endpoint,
                                                     ResponseCode response_code)
    : ClientResponse(endpoint, MessageType::CANCEL_OBSERVATION,
                     InterfaceType::INFORMATION_REPORTING,
                     unordered_set<ResponseCode>{
                         ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
                         ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                         ResponseCode::METHOD_NOT_ALLOWED},
                     response_code) {
  checkResponseCode(response_code);
}

string CancelObservationResponse::name() { return "CancelObservationResponse"; }
} // namespace LwM2M
