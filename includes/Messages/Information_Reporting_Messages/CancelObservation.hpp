#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
struct CancelObservationRequest : ServerRequest {
  const ElementIdVariant target_id_;

  CancelObservationRequest(EndpointPtr endpoint, ElementIdVariant target_id)
      : ServerRequest(endpoint, MessageType::CANCEL_OBSERVATION,
                      InterfaceType::INFORMATION_REPORTING),
        target_id_(target_id) {}

  std::string name() override final { return "CancelObservationRequest"; }
};

using CancelObservationRequestPtr = std::shared_ptr<CancelObservationRequest>;

struct CancelObservationResponse : ClientResponse {
  CancelObservationResponse(EndpointPtr endpoint, ResponseCode response_code)
      : ClientResponse(endpoint, MessageType::CANCEL_OBSERVATION,
                       InterfaceType::INFORMATION_REPORTING,
                       std::unordered_set<ResponseCode>{
                           ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
                           ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                           ResponseCode::METHOD_NOT_ALLOWED},
                       response_code) {
    checkResponseCode(response_code);
  }

  std::string name() override final { return "CancelObservationResponse"; }
};

using CancelObservationResponsePtr = std::shared_ptr<CancelObservationResponse>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_MESSAGE_HPP