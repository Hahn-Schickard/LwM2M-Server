#ifndef __LWM2M_DEVICE_MANAGMENT_CREATE_REQUEST_HPP
#define __LWM2M_DEVICE_MANAGMENT_CREATE_REQUEST_HPP

#include "DeviceManagmentInterfaceMessage.hpp"

namespace LwM2M {
struct Create_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::string value_;

  Create_Request(std::string endpoint_address, unsigned int endpoint_port,
                 unsigned int object_id, std::string value);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_CREATE_REQUEST_HPP