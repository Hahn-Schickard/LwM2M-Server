#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_SEND_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_SEND_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
struct SendResponse : ServerResponse {
  SendResponse(EndpointPtr endpoint, ResponseCode response_code)
      : ServerResponse(endpoint, MessageType::SEND,
                       InterfaceType::INFORMATION_REPORTING,
                       std::unordered_set<ResponseCode>{
                           ResponseCode::CHANGED, ResponseCode::BAD_REQUEST,
                           ResponseCode::NOT_FOUND},
                       response_code) {
    checkResponseCode(response_code);
  }

  std::string name() override final { return "SendResponse"; }
};

using SendResponsePtr = std::shared_ptr<SendResponse>;

struct SendRequest : ClientRequest {
  TargetContent content_;

  SendRequest(EndpointPtr endpoint, TargetContent content)
      : ClientRequest(endpoint, MessageType::SEND,
                      InterfaceType::INFORMATION_REPORTING),
        content_(content) {}

  std::string name() override final { return "SendRequest"; }

  SendResponsePtr makeResponse(ResponseCode response_code) {
    return std::make_shared<SendResponse>(endpoint_, response_code);
  }
};

using SendRequestPtr = std::shared_ptr<SendRequest>;

}; // namespace LwM2M

} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_SEND_MESSAGE_HPP