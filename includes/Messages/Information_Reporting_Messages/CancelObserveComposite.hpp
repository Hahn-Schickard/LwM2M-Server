#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
struct CancelObserveCompositeRequest : ServerRequest {
  const std::vector<ElementIdVariant> target_ids_;

  CancelObserveCompositeRequest(EndpointPtr endpoint,
                                std::vector<ElementIdVariant> target_ids)
      : ServerRequest(endpoint, MessageType::CANCEL_OBSERVE_COMPOSITE,
                      InterfaceType::INFORMATION_REPORTING) {}

  std::string name() override final { return "CancelObserveCompositeRequest"; }
};

using CancelObserveCompositeRequestPtr =
    std::shared_ptr<CancelObserveCompositeRequest>;

struct CancelObserveCompositeResponse : ClientResponse {
  std::vector<TargetContent> content_;

  ObserveCompositeResponse(EndpointPtr endpoint, ResponseCode response_code,
                           std::vector<TargetContent> conent)
      : ClientResponse(endpoint, MessageType::CANCEL_OBSERVE_COMPOSITE,
                       InterfaceType::INFORMATION_REPORTING,
                       std::unordered_set<ResponseCode>{
                           ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
                           ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                           ResponseCode::METHOD_NOT_ALLOWED},
                       response_code),
        content_(content) {
    checkResponseCode(response_code);
  }

  std::string name() override final { return "CancelObserveCompositeResponse"; }
};

using CancelObserveCompositeResponsePtr =
    std::shared_ptr<CancelObserveCompositeResponse>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP