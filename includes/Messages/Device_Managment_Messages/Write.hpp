#ifndef __LWM2M_DEVICE_MANAGMENT_WRITE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_WRITE_MESSAGE_HPP

#include "DeviceManagmentMessage.hpp"

namespace LwM2M {
/**
 * @brief sed to change the value of a Resource, the value of a Resource
 * Instance, the values of an arrayof Resources Instances or the values of
 * multiple Resources from an Object Instance as well as request the deletion or
 * the allocation of specific Instances of a Multiple-Instance Resource
 *
 * Content MUST be formated in application/vnd.oma.lwm2m+tlv Type Length Value,
 * application/senml+json or application/senml+cbor Sensor Measurement Lists
 * data formats
 */
struct WriteRequest : public DeviceManagmentRequest {
  EelmentIdVariant target_id_;
  DataFormatPtr content_;

  WriteRequest(EndpointPtr endpoint, EelmentIdVariant target_id,
               DataFormatPtr content);

  std::string name() override final;
};

using WriteRequestPtr = std::shared_ptr<WriteRequest>;

/**
 * @brief Response to LwM2M::WriteRequest, indicates wheater the request was
 * succefull, requires more data or failed
 *
 * Supported response codes:
 * ResponseCode::CHANGED
 * ResponseCode::CONTINUE
 * ResponseCode::BAD_REQUEST
 * ResponseCode::UNAUTHORIZED
 * ResponseCode::NOT_FOUND
 * ResponseCode::METHOD_NOT_ALLOWED
 * ResponseCode::NOT_ACCEPTABLE
 * ResponseCode::REQUEST_ENTITY_INCOMPLETE
 * ResponseCode::REQUEST_ENTITY_TOO_LARGE
 */
struct WriteResponse : DeviceManagmentResponse {
  WriteResponse(EndpointPtr endpoint, ResponseCode response_code);

  std::string name() override final;
};

using WriteResponsePtr = std::shared_ptr<WriteResponse>;

} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_WRITE_MESSAGE_HPP