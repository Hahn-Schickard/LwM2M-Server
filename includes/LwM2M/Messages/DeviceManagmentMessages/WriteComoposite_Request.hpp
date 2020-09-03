#ifndef __LWM2M_DEVICE_MANAGMENT_WRITE_COMPOSITE_REQUEST_HPP
#define __LWM2M_DEVICE_MANAGMENT_WRITE_COMPOSITE_REQUEST_HPP

#include "DeviceManagmentInterfaceMessage.hpp"

namespace LwM2M {
struct WriteComoposite_Request : public DeviceManagment_Interface_Message {
  WriteComoposite_Request(std::string endpoint_address,
                          unsigned int endpoint_port);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_WRITE_COMPOSITE_REQUEST_HPP