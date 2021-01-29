#ifndef __LWM2M_DEVICE_MANAGMENT_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
struct DeviceManagmentRequest : ServerRequest {
  DeviceManagmentRequest(EndpointPtr endpoint, MessageType message_type)
      : ServerRequest(endpoint, message_type, InterfaceType::DEVICE_MANAGMENT) {
  }
};

using DeviceManagmentRequestPtr = std::shared_ptr<DeviceManagmentRequest>;

struct DeviceManagmentResponse : ClientResponse {
  DeviceManagmentResponse(EndpointPtr endpoint, MessageType message_type,
                          std::unordered_set<ResponseCode> supported_responses,
                          ResponseCode response_code)
      : ClientResponse(endpoint, message_type, InterfaceType::DEVICE_MANAGMENT,
                       supported_responses, response_code) {}
};

using DeviceManagmentResponsePtr = std::shared_ptr<DeviceManagmentResponse>;

} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_MESSAGE_HPP