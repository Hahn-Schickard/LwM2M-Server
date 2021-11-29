#ifndef __LWM2M_INFORMATION_REPORTING_INTERFACE_MESSAGE_HPP
#define __LWM2M_INFORMATION_REPORTING_INTERFACE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Generalizes all of the LwM2M Information Reporting Interface Requests.
 *
 * @see CancelObservationRequest
 * @see CancelObserveCompositeRequest
 * @see ObserveRequest
 * @see ObserveCompositeRequest
 *
 * All of the messages, that implement this type are Downlink (Server is sending
 * this message to the Device)
 *
 * @attention This is an abstract class of Message. It can not be built or exist
 * on its own. It must always have a concrete implementation. Accessing this
 * class without concrete implementation will cause undefined behaviour.
 */
struct InformationReportingRequest : ServerRequest {
protected:
  InformationReportingRequest(EndpointPtr endpoint, MessageType message_type,
                              PayloadPtr payload = PayloadPtr())
      : ServerRequest(endpoint, message_type,
                      InterfaceType::INFORMATION_REPORTING, payload) {}
};

/**
 * @brief Generalizes all of the LwM2M Information Reporting Interface Requests.
 *
 * @see SendResponse
 *
 * All of the messages, that implement this type are Downlink (Server is sending
 * this message to the Device)
 *
 * @attention This is an abstract class of Message. It can not be built or exist
 * on its own. It must always have a concrete implementation. Accessing this
 * class without concrete implementation will cause undefined behaviour.
 */
struct InformationReportingDownlinkResponse : ServerResponse {
protected:
  InformationReportingDownlinkResponse(
      EndpointPtr endpoint, MessageType message_type,
      std::unordered_set<ResponseCode> supported_responses,
      ResponseCode response_code, PayloadPtr payload = PayloadPtr())
      : ServerResponse(endpoint, message_type,
                       InterfaceType::INFORMATION_REPORTING,
                       supported_responses, response_code, payload) {}
};

/**
 * @brief Generalizes all of the LwM2M Information Reporting Interface Requests.
 *
 * @see SendRequest
 *
 * All of the messages, that implement this type are Uplink (Device is sending
 * this message to the Server)
 *
 * @attention This is an abstract class of Message. It can not be built or exist
 * on its own. It must always have a concrete implementation. Accessing this
 * class without concrete implementation will cause undefined behaviour.
 */
struct InformationReportingDownlinkRequest : ClientRequest {
protected:
  InformationReportingDownlinkRequest(EndpointPtr endpoint,
                                      MessageType message_type,
                                      PayloadPtr payload = PayloadPtr())
      : ClientRequest(endpoint, message_type,
                      InterfaceType::INFORMATION_REPORTING, payload) {}
};

using InformationReportingRequestPtr =
    std::shared_ptr<InformationReportingRequest>;
using InformationReportingDownlinkResponsePtr =
    std::shared_ptr<InformationReportingDownlinkResponse>;
using InformationReportingDownlinkRequestPtr =
    std::shared_ptr<InformationReportingDownlinkRequest>;
} // namespace LwM2M

#endif //__LWM2M_INFORMATION_REPORTING_INTERFACE_MESSAGE_HPP