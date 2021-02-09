#include "Send.hpp"

using namespace std;

namespace LwM2M {

SendResponse::SendResponse(EndpointPtr endpoint, ResponseCode response_code)
    : ServerResponse(endpoint, MessageType::SEND,
                     InterfaceType::INFORMATION_REPORTING,
                     unordered_set<ResponseCode>{ResponseCode::CHANGED,
                                                 ResponseCode::BAD_REQUEST,
                                                 ResponseCode::NOT_FOUND},
                     response_code) {
  checkResponseCode(response_code);
}

string SendResponse::name() { return "SendResponse"; }

SendRequest::SendRequest(EndpointPtr endpoint, TargetContent content)
    : ClientRequest(endpoint, MessageType::SEND,
                    InterfaceType::INFORMATION_REPORTING),
      content_(content) {}

string SendRequest::name() { return "SendRequest"; }

SendResponsePtr SendRequest::makeResponse(ResponseCode response_code) {
  return make_shared<SendResponse>(endpoint_, response_code);
}

} // namespace LwM2M
