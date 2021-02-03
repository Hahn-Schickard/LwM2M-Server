#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used stop the observation of a given element within the Client.
 *
 */
struct CancelObservationRequest : ServerRequest {
  const EelmentIdVariant target_id_;

  CancelObservationRequest(EndpointPtr endpoint, EelmentIdVariant target_id);

  std::string name() override final;
};

using CancelObservationRequestPtr = std::shared_ptr<CancelObservationRequest>;

/**
 * @brief Response to LwM2M::CancelObservationRequest, indicates wheater the
 * request was succefull, requires more data or failed
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
struct CancelObservationResponse : ClientResponse {
  CancelObservationResponse(EndpointPtr endpoint, ResponseCode response_code);

  std::string name() override final;
};

using CancelObservationResponsePtr = std::shared_ptr<CancelObservationResponse>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_CANCEL_OBSERVE_MESSAGE_HPP