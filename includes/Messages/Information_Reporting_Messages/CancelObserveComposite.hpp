#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used stop the observation of a given elements within the Client.
 *
 */
struct CancelObserveCompositeRequest : ServerRequest {
  const std::vector<EelmentIdVariant> target_ids_;

  CancelObserveCompositeRequest(EndpointPtr endpoint,
                                std::vector<EelmentIdVariant> target_ids);

  std::string name() override final;
};

using CancelObserveCompositeRequestPtr =
    std::shared_ptr<CancelObserveCompositeRequest>;

/**
 * @brief Response to LwM2M::CancelObserveCompositeRequest, indicates wheater
 * the request was succefull, requires more data or failed
 *
 * Supported response codes:
 * - ResponseCode::CONTENT - Operation was a success.
 * - ResponseCode::BAD_REQUEST - Client encountered an undetermened error, while
 * processing the request.
 * - ResponseCode::UNAUTHORIZED - Access rights permission denied.
 * - ResponseCode::NOT_FOUND - Target EelmentIdVariant does not point to a valid
 * element within the client.
 * - ResponseCode::METHOD_NOT_ALLOWED Target EelmentIdVariant is not allowed
 * to use Cancel Observe operation.
 */
struct CancelObserveCompositeResponse : ClientResponse {
  std::vector<TargetContent> content_;

  CancelObserveCompositeResponse(EndpointPtr endpoint,
                                 ResponseCode response_code,
                                 std::vector<TargetContent> conent);

  std::string name() override final;
};

using CancelObserveCompositeResponsePtr =
    std::shared_ptr<CancelObserveCompositeResponse>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_COMPOSITE_MESSAGE_HPP