#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
struct ObserveRequest : ServerRequest {
  const ElementIdVariant target_id_;

  ObserveRequest(EndpointPtr endpoint, ElementIdVariant target_id)
      ServerRequest(endpoint, MessageType::OBSERVE,
                    InterfaceType::INFORMATION_REPORTING),
      target_id_(target_id) {}

  std::string name() override final { return "ObserveRequest"; }
};

using ObserveRequestPtr = std::shared_ptr<ObserveRequest>;

struct ObservResponse : ClientResponse {
  const DataFormatPtr content_;

  ObservResponse(EndpointPtr endpoint, ResponseCode response_code,
                 DataFormatPtr content)
      : ClientResponse(endpoint, MessageType::OBSERVE,
                       InterfaceType::INFORMATION_REPORTING,
                       std::unordered_set<ResponseCode>{
                           ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
                           ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                           ResponseCode::METHOD_NOT_ALLOWED},
                       response_code),
        content_(content) {
    checkResponseCode(response_code);
  }

  std::string name() override final { return "ObservResponse"; }
};

using ObservResponsePtr = std::shared_ptr<ObservResponse>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP