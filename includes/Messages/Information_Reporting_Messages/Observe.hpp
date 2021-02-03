#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to start observing a given element within the client.
 *
 */
struct ObserveRequest : ServerRequest {
  const EelmentIdVariant target_id_;

  ObserveRequest(EndpointPtr endpoint, EelmentIdVariant target_id);

  std::string name() override final;
};

using ObserveRequestPtr = std::shared_ptr<ObserveRequest>;

/**
 * @brief Response to LwM2M::ObserveRequest, indicates wheater
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
struct ObservResponse : ClientResponse {
  const DataFormatPtr content_;

  ObservResponse(EndpointPtr endpoint, ResponseCode response_code,
                 DataFormatPtr content);

  std::string name() override final;
};

using ObservResponsePtr = std::shared_ptr<ObservResponse>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP