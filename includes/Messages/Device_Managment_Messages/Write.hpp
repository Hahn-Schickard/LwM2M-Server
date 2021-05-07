#ifndef __LWM2M_DEVICE_MANAGEMENT_WRITE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_WRITE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to change the value of a Resource, the value of a Resource
 * Instance, the values of an array of Resources Instances or the values of
 * multiple Resources from an Object Instance as well as request the deletion or
 * the allocation of specific Instances of a Multiple-Instance Resource
 *
 * Content MUST be formated in application/vnd.oma.lwm2m+tlv Type Length Value,
 * application/senml+json or application/senml+cbor Sensor Measurement Lists
 * data formats
 */
struct WriteRequest : ServerRequest {
  WriteRequest(EndpointPtr endpoint, ElmentIdVariant target_id = ObjectID(0),
               DataFormatPtr content = DataFormatPtr());

  std::string name() override final;
};

using WriteRequestPtr = std::shared_ptr<WriteRequest>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_WRITE_MESSAGE_HPP