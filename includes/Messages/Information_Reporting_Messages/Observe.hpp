#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to start observing a given element within the client.
 *
 */
struct ObserveRequest : ServerRequest {
  ObserveRequest(EndpointPtr endpoint, ElmentIdVariant target_id = ObjectID(0));

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
 * - ResponseCode::NOT_FOUND - Target LwM2M::ElmentIdVariant does not point to a
 * valid element within the client.
 * - ResponseCode::METHOD_NOT_ALLOWED - Target LwM2M::ElmentIdVariant is not
 * allowed to use Cancel Observe operation.
 */
struct ObserveResponse : ClientResponse {
  ObserveResponse(EndpointPtr endpoint, ResponseCode response_code,
                  DataFormatPtr content);

  ObserveResponse(EndpointPtr endpoint,
                  ResponseCode response_code = ResponseCode::BAD_REQUEST);

  std::string name() override final;
};

using ObservResponsePtr = std::shared_ptr<ObserveResponse>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_OBSERVE_MESSAGE_HPP