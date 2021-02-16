#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP

#include "Message.hpp"

#include <vector>

namespace LwM2M {
/**
 * @brief Used to start observing the given elements within the Client.
 *
 */
struct ObserveCompositeRequest : ServerRequest {
  ObserveCompositeRequest(
      EndpointPtr endpoint,
      std::vector<ElmentIdVariant> target_ids = std::vector<ElmentIdVariant>());

  std::string name() override final;
};

using ObserveCompositeRequestPtr = std::shared_ptr<ObserveCompositeRequest>;

/**
 * @brief Response to LwM2M::ObserveCompositeRequest, indicates wheater
 * the request was succefull, requires more data or failed
 *
 * Supported response codes:
 * - ResponseCode::CONTENT - Operation was a success.
 * - ResponseCode::BAD_REQUEST - Client encountered an undetermened error, while
 * processing the request.
 * - ResponseCode::UNAUTHORIZED - Access rights permission denied.
 * - ResponseCode::NOT_FOUND - None of target LwM2M::ElmentIdVariant instances
 * point to a valid element within the client.
 * - ResponseCode::METHOD_NOT_ALLOWED - None of target LwM2M::ElmentIdVariant
 * instances are allowed to use Cancel Observe operation.
 */
struct ObserveCompositeResponse : ClientResponse {
  ObserveCompositeResponse(EndpointPtr endpoint, ResponseCode response_code,
                           std::vector<TargetContent> conent);

  ObserveCompositeResponse(EndpointPtr endpoint, ResponseCode response_code =
                                                     ResponseCode::BAD_REQUEST);

  std::string name() override final;
};

using ObserveCompositeResponsePtr = std::shared_ptr<ObserveCompositeResponse>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_COMPOSITE_MESSAGE_HPP