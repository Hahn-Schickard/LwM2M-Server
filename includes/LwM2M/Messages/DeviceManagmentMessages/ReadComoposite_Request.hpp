#ifndef __LWM2M_DEVICE_MANAGMENT_READ_COMPOSITE_REQUEST_HPP
#define __LWM2M_DEVICE_MANAGMENT_READ_COMPOSITE_REQUEST_HPP

#include "DeviceManagmentInterfaceMessage.hpp"

namespace LwM2M {
struct ReadComoposite_Request : public DeviceManagment_Interface_Message {
  ReadComoposite_Request(std::string endpoint_address,
                         unsigned int endpoint_port);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_READ_COMPOSITE_REQUEST_HPP