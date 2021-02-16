#ifndef __LWM2M_DEVICE_MANAGMENT_CREATE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_CREATE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to create Object Instance(s) within the LwM2M
 * Client. It MUST target an Object.
 *
 * The new value included in the payload MUST be formated in
 * application/vnd.oma.lwm2m+tlv Type Length Value, application/senml+json or
 * application/senml+cbor Sensor Measurement Lists data formats
 *
 */
struct CreateRequest : public ServerRequest {
  CreateRequest(EndpointPtr endpoint, ObjectID target_id = ObjectID(0),
                DataFormatPtr content = DataFormatPtr());
  std::string name() override final;
};

using CreateRequestPtr = std::shared_ptr<CreateRequest>;

/**
 * @brief Response to LwM2M::CreateRequest, indicates wheater the request was
 * succefull, requires more data or failed
 *
 * Supported response codes:
 * - ResponseCode::CREATED - Operation was a success.
 * - ResponseCode::BAD_REQUEST - Target ObjectID does not support multpile
 * instances or Content Format was not specified.
 * - ResponseCode::UNAUTHORIZED - Access rights permission denied.
 * - ResponseCode::NOT_FOUND - ObjectID does not point to a valid object.
 * - ResponseCode::METHOD_NOT_ALLOWED - Target ObjectID is not allowed to use
 * Create operation.
 * - ResponseCode::UNSUPPORTED_CONTENT_FORMAT - Specified Content Format is not
 * supported by the client.
 */
struct CreateResponse : ClientResponse {
  CreateResponse(EndpointPtr endpoint,
                 ResponseCode response_code = ResponseCode::BAD_REQUEST);

  std::string name() override final;
}; // namespace LwM2M

using CreateResponsePtr = std::shared_ptr<CreateResponse>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_CREATE_MESSAGE_HPP