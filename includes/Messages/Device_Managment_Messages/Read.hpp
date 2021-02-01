#ifndef __LWM2M_DEVICE_MANAGMENT_READ_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_READ_MESSAGE_HPP

#include "DeviceManagmentMessage.hpp"

namespace LwM2M {
/**
 * @brief Used to access the value of a Resource, a Resource Instance, an array
 * of Resource Instances, anObject Instance or all the Object Instances of an
 * Object.
 *
 */
struct ReadRequest : DeviceManagmentRequest {
  EelmentIdVariant target_id_;

  ReadRequest(EndpointPtr endpoint, EelmentIdVariant target_id);

  std::string name() override final;
};

using ReadRequestPtr = std::shared_ptr<ReadRequest>;

/**
 * @brief Response to LwM2M::ReadRequest, contains a list of
 * application/vnd.oma.lwm2m+tlv Type Length Values for each targeted Object,
 * Object Instance, Resource or Resource Instance
 *
 * Supported response codes:
 * ResponseCode::CONTENT
 * ResponseCode::BAD_REQUEST
 * ResponseCode::UNAUTHORIZED
 * ResponseCode::NOT_FOUND
 * ResponseCode::METHOD_NOT_ALLOWED
 * ResponseCode::NOT_ACCEPTABLE
 */
struct ReadResponse : DeviceManagmentResponse {
  DataFormatPtr content_;

  ReadResponse(EndpointPtr endpoint, ResponseCode response_code,
               DataFormatPtr content);

  std::string name() override final;
};

using ReadResponsePtr = std::shared_ptr<ReadResponse>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_READ_MESSAGE_HPP