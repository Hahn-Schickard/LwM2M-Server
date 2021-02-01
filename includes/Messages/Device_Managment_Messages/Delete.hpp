#ifndef __LWM2M_DEVICE_MANAGMENT_DELETE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGMENT_DELETE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to delete an Object Instance within the LwM2M Client
 *
 */
struct DeleteRequest : public ServerRequest {
  const ObjectInstanceID target_id_;

  DeleteRequest(EndpointPtr endpoint, ObjectInstanceID target_id);

  std::string name() override final;
};

using DeleteRequestPtr = std::shared_ptr<DeleteRequest>;

/**
 * @brief Response to LwM2M::DeleteRequest, indicates wheater the request was
 * succefull, requires more data or failed
 *
 * Supported response codes:
 * ResponseCode::DELETED
 * ResponseCode::BAD_REQUEST
 * ResponseCode::UNAUTHORIZED,
 * ResponseCode::NOT_FOUND
 * ResponseCode::METHOD_NOT_ALLOWED
 */
struct DeleteResponse : ClientResponse {
  DeleteResponse(EndpointPtr endpoint, ResponseCode response_code);

  std::string name() override final;
};

using DeleteResponsePtr = std::shared_ptr<DeleteResponse>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_DELETE_MESSAGE_HPP