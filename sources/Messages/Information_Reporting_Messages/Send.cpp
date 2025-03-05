#include "Send.hpp"

using namespace std;

namespace LwM2M {

SendResponse::SendResponse(EndpointPtr endpoint, // NOLINT
    ResponseCode response_code)
    : InformationReportingDownlinkResponse(endpoint, // NOLINT
          MessageType::Send,
          unordered_set<ResponseCode>{ResponseCode::Changed,
              ResponseCode::Bad_Request, ResponseCode::Not_Found},
          response_code) {
  checkResponseCode(response_code);
}

string SendResponse::name() const { return "SendResponse"; }

SendRequest::SendRequest(EndpointPtr endpoint, // NOLINT
    TargetContent content) // NOLINT
    : InformationReportingDownlinkRequest(endpoint, // NOLINT
          MessageType::Send, make_shared<Payload>(content)) {}

string SendRequest::name() const { return "SendRequest"; }

SendResponsePtr SendRequest::makeResponse(ResponseCode response_code) {
  return make_shared<SendResponse>(endpoint_, response_code);
}

} // namespace LwM2M
