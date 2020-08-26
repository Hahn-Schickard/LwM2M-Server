#ifndef __LWM2M_DEVICE_MANAGMENT_READ_REQUEST_HPP
#define __LWM2M_DEVICE_MANAGMENT_READ_REQUEST_HPP

#include "DeviceManagmentInterfaceMessage.hpp"

namespace LwM2M {
struct Read_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;

  Read_Request(std::string endpoint_address, unsigned int endpoint_port,
               unsigned int object_id, unsigned int object_instance_id);
  Read_Request(std::string endpoint_address, unsigned int endpoint_port,
               unsigned int object_id, unsigned int object_instance_id,
               unsigned int resource_id);
  Read_Request(std::string endpoint_address, unsigned int endpoint_port,
               unsigned int object_id, unsigned int object_instance_id,
               unsigned int resource_id, unsigned int resoruce_instance_id);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_READ_REQUEST_HPP