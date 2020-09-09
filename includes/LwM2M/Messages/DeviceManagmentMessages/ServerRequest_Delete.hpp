#ifndef __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_DELETE_HPP
#define __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_DELETE_HPP

#include "DeviceManagmentInterfaceMessage.hpp"

namespace LwM2M {
struct ServerRequest_Delete : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  unsigned int object_instance_id_;

  ServerRequest_Delete(std::string endpoint_address, unsigned int endpoint_port,
                       unsigned int object_id, unsigned int object_instance_id);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_DELETE_HPP