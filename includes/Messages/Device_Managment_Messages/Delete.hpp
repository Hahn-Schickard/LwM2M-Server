#ifndef __LWM2M_DEVICE_MANAGEMENT_DELETE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_DELETE_MESSAGE_HPP

#include "Message.hpp"

namespace LwM2M {
/**
 * @brief Used to delete an Object Instance within the LwM2M Client
 *
 */
struct DeleteRequest : public ServerRequest {
  DeleteRequest(EndpointPtr endpoint, ObjectID target_id = ObjectID(0));

  std::string name() override final;
};

using DeleteRequestPtr = std::shared_ptr<DeleteRequest>;
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_DELETE_MESSAGE_HPP