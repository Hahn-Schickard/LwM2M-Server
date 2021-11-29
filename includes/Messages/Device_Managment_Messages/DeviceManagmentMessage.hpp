#ifndef __LWM2M_DEVICE_MANAGEMENT_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {

/**
 * @brief Generalizes all of the LwM2M Device Management Interface Requests.
 *
 * @see CreateRequest
 * @see DeleteRequest
 * @see DiscoverRequest
 * @see ExecuteRequest
 * @see ReadRequest
 * @see ReadCompositeRequest
 * @see WriteRequest
 * @see WriteAttributesRequest
 * @see WriteCompositeRequest
 *
 * All of the messages, that implement this type are Downlink (Server is sending
 * this message to the Device)
 *
 * @attention This is an abstract class of Message. It can not be built or exist
 * on its own. It must always have a concrete implementation. Accessing this
 * class without concrete implementation will cause undefined behaviour.
 */
struct DeviceManagementRequest : ServerRequest {
protected:
  DeviceManagementRequest(EndpointPtr endpoint, MessageType message_type,
                          PayloadPtr payload = PayloadPtr())
      : ServerRequest(endpoint, message_type, InterfaceType::DEVICE_MANAGEMENT,
                      payload) {}
};

using DeviceManagementRequestPtr = std::shared_ptr<DeviceManagementRequest>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_MESSAGE_HPP