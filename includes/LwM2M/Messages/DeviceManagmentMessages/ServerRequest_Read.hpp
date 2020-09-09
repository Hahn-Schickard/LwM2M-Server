#ifndef __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_READ_HPP
#define __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_READ_HPP

#include "DeviceManagmentMessage.hpp"

namespace LwM2M {
struct ServerRequest_Read : public DeviceManagmentMessage {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;

  ServerRequest_Read(std::string endpoint_address, unsigned int endpoint_port,
                     unsigned int object_id, unsigned int object_instance_id);
  ServerRequest_Read(std::string endpoint_address, unsigned int endpoint_port,
                     unsigned int object_id, unsigned int object_instance_id,
                     unsigned int resource_id);
  ServerRequest_Read(std::string endpoint_address, unsigned int endpoint_port,
                     unsigned int object_id, unsigned int object_instance_id,
                     unsigned int resource_id,
                     unsigned int resoruce_instance_id);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_READ_HPP