#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP

#include "Message.hpp"

#include <vector>

namespace LwM2M {
struct ObserveCompositeRequest : ServerRequest {
  const std::vector<ElementIdVariant> target_ids_;

  ObserveCompositeRequest(EndpointPtr endpoint,
                          std::vector<ElementIdVariant> target_ids)
      : ServerRequest(endpoint, MessageType::OBSERVE_COMPOSITE,
                      InterfaceType::INFORMATION_REPORTING) {}

  std::string name() override final { return "ObserveCompositeRequest"; }
};

using ObserveCompositeRequestPtr = std::shared_ptr<ObserveCompositeRequest>;
;

struct ObserveCompositeResponse : ClientResponse {
  std::vector<TargetContent> content_;

  ObserveCompositeResponse(EndpointPtr endpoint, ResponseCode response_code,
                           std::vector<TargetContent> conent)
      : ClientResponse(endpoint, MessageType::CANCEL_OBSERVATION,
                       InterfaceType::INFORMATION_REPORTING,
                       std::unordered_set<ResponseCode>{
                           ResponseCode::CONTENT, ResponseCode::BAD_REQUEST,
                           ResponseCode::UNAUTHORIZED, ResponseCode::NOT_FOUND,
                           ResponseCode::METHOD_NOT_ALLOWED,
                           ResponseCode::UNSUPPORTED_CONTENT_FORMAT},
                       response_code),
        content_(content) {
    checkResponseCode(response_code);
  }

  std::string name() override final { return "ObserveCompositeResponse"; }
};

using ObserveCompositeResponse = std::shared_ptr<ObserveCompositeResponse>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP