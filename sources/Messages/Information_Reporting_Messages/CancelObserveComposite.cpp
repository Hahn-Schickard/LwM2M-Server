#include "CancelObserveComposite.hpp"

using namespace std;

namespace LwM2M {
CancelObserveCompositeRequest::CancelObserveCompositeRequest(
    EndpointPtr endpoint, vector<ElmentIdVariant> target_ids)
    : ServerRequest(endpoint, MessageType::CANCEL_OBSERVATION_COMPOSITE,
                    InterfaceType::INFORMATION_REPORTING),
      target_ids_(target_ids) {}

string CancelObserveCompositeRequest::name() {
  return "CancelObserveCompositeRequest";
}

CancelObserveCompositeResponse::CancelObserveCompositeResponse(
    EndpointPtr endpoint, ResponseCode response_code,
    vector<TargetContent> content)
    : ClientResponse(endpoint, MessageType::CANCEL_OBSERVATION_COMPOSITE,
                     InterfaceType::INFORMATION_REPORTING,
                     unordered_set<ResponseCode>{
                         ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
                         ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                         ResponseCode::METHOD_NOT_ALLOWED},
                     response_code),
      content_(content) {
  checkResponseCode(response_code);
}

string CancelObserveCompositeResponse::name() {
  return "CancelObserveCompositeResponse";
}
} // namespace LwM2M
