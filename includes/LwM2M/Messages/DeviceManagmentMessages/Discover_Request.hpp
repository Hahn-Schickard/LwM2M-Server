#ifndef __LWM2M_DEVICE_MANAGMENT_DISCOVER_REQUEST_HPP
#define __LWM2M_DEVICE_MANAGMENT_DISCOVER_REQUEST_HPP

#include "DeviceManagmentInterfaceMessage.hpp"

namespace LwM2M {
struct Discover_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;

  Discover_Request(std::string endpoint_address, unsigned int endpoint_port,
                   unsigned int object_id,
                   std::optional<unsigned int> object_instance_id,
                   std::optional<unsigned int> resource_id);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_DISCOVER_REQUEST_HPP