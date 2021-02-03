#include "Observe.hpp"

using namespace std;

namespace LwM2M {

ObserveRequest::ObserveRequest(EndpointPtr endpoint, EelmentIdVariant target_id)
    : ServerRequest(endpoint, MessageType::OBSERVE,
                    InterfaceType::INFORMATION_REPORTING),
      target_id_(target_id) {}

string ObserveRequest::name() { return "ObserveRequest"; }

ObservResponse::ObservResponse(EndpointPtr endpoint, ResponseCode response_code,
                               DataFormatPtr content)
    : ClientResponse(endpoint, MessageType::OBSERVE,
                     InterfaceType::INFORMATION_REPORTING,
                     unordered_set<ResponseCode>{
                         ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
                         ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                         ResponseCode::METHOD_NOT_ALLOWED},
                     response_code),
      content_(content) {
  checkResponseCode(response_code);
}

string ObservResponse::name() { return "ObservResponse"; }

} // namespace LwM2M
