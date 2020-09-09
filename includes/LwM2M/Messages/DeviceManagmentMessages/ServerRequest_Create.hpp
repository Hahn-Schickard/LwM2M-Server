#ifndef __LWM2M_DEVICE_MANAGMENT_SERVER_REQUESTCREATE_HPP
#define __LWM2M_DEVICE_MANAGMENT_SERVER_REQUESTCREATE_HPP

#include "DeviceManagmentInterfaceMessage.hpp"

namespace LwM2M {
struct ServerRequest_Create : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::string value_;

  ServerRequest_Create(std::string endpoint_address, unsigned int endpoint_port,
                       unsigned int object_id, std::string value);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_SERVER_REQUESTCREATE_HPP