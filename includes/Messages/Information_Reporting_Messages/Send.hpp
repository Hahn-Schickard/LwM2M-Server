#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_SEND_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_SEND_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Response to LwM2M::SendRequest, indicates wheater
 * the request was succefull, requires more data or failed
 *
 * Supported response codes:
 * - ResponseCode::CHANGED - Operation was a success.
 * - ResponseCode::BAD_REQUEST - Given LwM2M::DataFormatPtr could not be
 * interpreted, or Server encountered an undetermened error.
 * - ResponseCode::NOT_FOUND - Target LwM2M::ElmentIdVariant does not point to a
 * valid element within the client.
 */
struct SendResponse : ServerResponse {
  SendResponse(EndpointPtr endpoint,
               ResponseCode response_code = ResponseCode::BAD_REQUEST);

  std::string name() override final;
};

using SendResponsePtr = std::shared_ptr<SendResponse>;

/**
 * @brief Used by the Client to send data value changes without prior request
 *
 */
struct SendRequest : ClientRequest {
  SendRequest(EndpointPtr endpoint,
              TargetContent content = TargetContent(ObjectID(0),
                                                    DataFormatPtr()));

  std::string name() override final;

  SendResponsePtr
  makeResponse(ResponseCode response_code = ResponseCode::BAD_REQUEST);
};

using SendRequestPtr = std::shared_ptr<SendRequest>;

} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_SEND_MESSAGE_HPP