#ifndef __LWM2M_DEVICE_MANAGMENT_READ_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_READ_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to access the value of a Resource, a Resource Instance, an array
 * of Resource Instances, anObject Instance or all the Object Instances of an
 * Object.
 *
 */
struct ReadRequest : ServerRequest {
  ReadRequest(EndpointPtr endpoint, ElmentIdVariant target_id = ObjectID(0));

  std::string name() override final;
};

using ReadRequestPtr = std::shared_ptr<ReadRequest>;

/**
 * @brief Response to LwM2M::ReadRequest, contains a list of
 * application/vnd.oma.lwm2m+tlv Type Length Values for each targeted Object,
 * Object Instance, Resource or Resource Instance
 *
 * Supported response codes:
 * - ResponseCode::CONTENT - Operation was a success.
 * - ResponseCode::BAD_REQUEST - Client encountered an undetermened error, while
 * processing the request.
 * - ResponseCode::UNAUTHORIZED - Access rights permission denied.
 * - ResponseCode::NOT_FOUND - Target LwM2M::ElmentIdVariant does not point to a
 * valid element within the client.
 * - ResponseCode::METHOD_NOT_ALLOWED - Target LwM2M::ElmentIdVariant is not
 * allowed to use Read operation.
 * - ResponseCode::NOT_ACCEPTABLE - None of preferred Content Formats are
 * supported by the Client
 */
struct ReadResponse : ClientResponse {
  ReadResponse(EndpointPtr endpoint, ResponseCode response_code,
               DataFormatPtr content);

  ReadResponse(EndpointPtr endpoint,
               ResponseCode response_code = ResponseCode::BAD_REQUEST);

  std::string name() override final;
};

using ReadResponsePtr = std::shared_ptr<ReadResponse>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_READ_MESSAGE_HPP