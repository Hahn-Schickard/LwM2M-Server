#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_SEND_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_SEND_MESSAGE_HPP

#include "InformationReporingMessage.hpp"

namespace LwM2M {
/**
 * @addtogroup Message_Model
 * @{
 */

/**
 * @brief Response to LwM2M::SendRequest, indicates if
 * the request was unsuccessful, requires more data or failed
 *
 * Supported response codes:
 * - ResponseCode::Changed - Operation was a success.
 * - ResponseCode::Bad_Request - Given LwM2M::DataFormatPtr could not be
 * interpreted, or Server encountered an undetermined error.
 * - ResponseCode::Not_Found - Target LwM2M::ElmentIdVariant does not point to a
 * valid element within the client.
 */
struct SendResponse : InformationReportingDownlinkResponse {
  SendResponse(EndpointPtr endpoint,
      ResponseCode response_code = ResponseCode::Bad_Request);

  std::string name() const final;
};

using SendResponsePtr = std::shared_ptr<SendResponse>;

/**
 * @brief Used by the Client to send data value changes without prior request
 *
 */
struct SendRequest : InformationReportingDownlinkRequest {
  SendRequest(EndpointPtr endpoint,
      TargetContent content = TargetContent(ElementID(0), DataFormatPtr()));

  std::string name() const final;

  SendResponsePtr makeResponse(
      ResponseCode response_code = ResponseCode::Bad_Request);
};

using SendRequestPtr = std::shared_ptr<SendRequest>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_SEND_MESSAGE_HPP