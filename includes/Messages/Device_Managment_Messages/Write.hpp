#ifndef __LWM2M_DEVICE_MANAGMENT_WRITE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_WRITE_MESSAGE_HPP

#include "Message.hpp"

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
struct WriteRequest : ServerRequest {
  const ElmentIdVariant target_id_;
  const DataFormatPtr content_;

  WriteRequest(EndpointPtr endpoint, ElmentIdVariant target_id = ObjectID(0),
               DataFormatPtr content = DataFormatPtr());

  std::string name() override final;
};

using WriteRequestPtr = std::shared_ptr<WriteRequest>;

/**
 * @brief Response to LwM2M::WriteRequest, indicates wheater the request was
 * succefull, requires more data or failed
 *
 * Supported response codes:
 * - ResponseCode::CHANGED - Operation was a success.
 * - ResponseCode::CONTINUE - Operation was a success, but requires more data
 * fragments.
 * - ResponseCode::BAD_REQUEST - Data format is written in wrong Content Format.
 * - ResponseCode::UNAUTHORIZED - Access rights permission denied.
 * - ResponseCode::NOT_FOUND - Target LwM2M::ElmentIdVariant does not point to a
 * valid element within the client.
 * - ResponseCode::METHOD_NOT_ALLOWED - Target LwM2M::ElmentIdVariant is not
 * allowed to use Write operation.
 * - ResponseCode::NOT_ACCEPTABLE - The specified Content Format type is not
 * supported by the Client.
 * - ResponseCode::REQUEST_ENTITY_INCOMPLETE - Failed to receive all of the
 * request fragments.
 * - ResponseCode::REQUEST_ENTITY_TOO_LARGE - Request exceeds the ammount of
 * allowed message fragments.
 */
struct WriteResponse : ClientResponse {
  WriteResponse(EndpointPtr endpoint,
                ResponseCode response_code = ResponseCode::BAD_REQUEST);

  std::string name() override final;
};

using WriteResponsePtr = std::shared_ptr<WriteResponse>;

} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_WRITE_MESSAGE_HPP