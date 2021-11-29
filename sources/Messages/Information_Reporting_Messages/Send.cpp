#include "Send.hpp"

using namespace std;

namespace LwM2M {

SendResponse::SendResponse(EndpointPtr endpoint, ResponseCode response_code)
    : InformationReportingDownlinkResponse(
          endpoint, MessageType::SEND,
          unordered_set<ResponseCode>{ResponseCode::CHANGED,
                                      ResponseCode::BAD_REQUEST,
                                      ResponseCode::NOT_FOUND},
          response_code) {
  checkResponseCode(response_code);
}

string SendResponse::name() { return "SendResponse"; }

SendRequest::SendRequest(EndpointPtr endpoint, TargetContent content)
    : InformationReportingDownlinkRequest(endpoint, MessageType::SEND,
                                          make_shared<Payload>(content)) {}

string SendRequest::name() { return "SendRequest"; }

SendResponsePtr SendRequest::makeResponse(ResponseCode response_code) {
  return make_shared<SendResponse>(endpoint_, response_code);
}

} // namespace LwM2M
