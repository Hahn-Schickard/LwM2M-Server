#ifndef __LWM2M_DEVICE_MANAGEMENT_CREATE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_CREATE_MESSAGE_HPP

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
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_CREATE_MESSAGE_HPP