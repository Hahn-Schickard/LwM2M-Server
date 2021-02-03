#include "ObserveComposite.hpp"

using namespace std;

namespace LwM2M {
ObserveCompositeRequest::ObserveCompositeRequest(
    EndpointPtr endpoint, vector<EelmentIdVariant> target_ids)
    : ServerRequest(endpoint, MessageType::OBSERVE_COMPOSITE,
                    InterfaceType::INFORMATION_REPORTING),
      target_ids_(target_ids) {}

string ObserveCompositeRequest::name() { return "ObserveCompositeRequest"; }

ObserveCompositeResponse::ObserveCompositeResponse(
    EndpointPtr endpoint, ResponseCode response_code,
    vector<TargetContent> content)
    : ClientResponse(endpoint, MessageType::CANCEL_OBSERVATION,
                     InterfaceType::INFORMATION_REPORTING,
                     unordered_set<ResponseCode>{
                         ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
                         ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                         ResponseCode::METHOD_NOT_ALLOWED,
                         ResponseCode::UNSUPPORTED_CONTENT_FORMAT},
                     response_code),
      content_(content) {
  checkResponseCode(response_code);
}

string ObserveCompositeResponse::name() { return "ObserveCompositeResponse"; }

} // namespace LwM2M
