#ifndef __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_COMPOSITE_HPP
#define __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_COMPOSITE_HPP

#include "DeviceManagmentMessage.hpp"

namespace LwM2M {
struct ServerRequest_WriteComoposite : public DeviceManagmentMessage {
  ServerRequest_WriteComoposite(std::string endpoint_address,
                                unsigned int endpoint_port);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_COMPOSITE_HPP