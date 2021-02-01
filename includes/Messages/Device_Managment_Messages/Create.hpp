#ifndef __LWM2M_DEVICE_MANAGMENT_CREATE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_CREATE_MESSAGE_HPP

#include "DeviceManagmentMessage.hpp"

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
struct CreateRequest : public DeviceManagmentRequest {
  const ObjectID target_id_;
  const DataFormatPtr content_;

  CreateRequest(EndpointPtr endpoint, ObjectID target_id,
                DataFormatPtr content);
  std::string name() override final;
};

using CreateRequestPtr = std::shared_ptr<CreateRequest>;

/**
 * @brief Response to LwM2M::CreateRequest, indicates wheater the request was
 * succefull, requires more data or failed
 *
 * Supported response codes:
 * ResponseCode::CREATED
 * ResponseCode::BAD_REQUEST
 * ResponseCode::UNAUTHORIZED,
 * ResponseCode::NOT_FOUND
 * ResponseCode::METHOD_NOT_ALLOWED
 * ResponseCode::NOT_ACCEPTABLE
 */
struct CreateResponse : DeviceManagmentResponse {
  CreateResponse(EndpointPtr endpoint, ResponseCode response_code);

  std::string name() override final;
}; // namespace LwM2M

using CreateResponsePtr = std::shared_ptr<CreateResponse>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_CREATE_MESSAGE_HPP