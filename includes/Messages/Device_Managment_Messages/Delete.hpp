#ifndef __LWM2M_DEVICE_MANAGEMENT_DELETE_MESSAGE_HPP
#define __LWM2M_DEVICE_MANAGEMENT_DELETE_MESSAGE_HPP

#include "DeviceManagmentMessage.hpp"

namespace LwM2M {
/**
 * @addtogroup Message_Model
 * @{
 */

/**
 * @brief Used to delete an Object Instance within the LwM2M Client
 *
 */
struct DeleteRequest : public DeviceManagementRequest {
  DeleteRequest(EndpointPtr endpoint, ElementID target_id = ElementID(0));

  std::string name() override final;
};

using DeleteRequestPtr = std::shared_ptr<DeleteRequest>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGEMENT_DELETE_MESSAGE_HPP