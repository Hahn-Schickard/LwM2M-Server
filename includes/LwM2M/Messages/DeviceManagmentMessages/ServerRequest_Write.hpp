#ifndef __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_HPP
#define __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_HPP

#include "DeviceManagmentInterfaceMessage.hpp"

namespace LwM2M {
struct ServerRequest_Write : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  unsigned int object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;
  std::string value_;

  ServerRequest_Write(std::string endpoint_address, unsigned int endpoint_port,
                      unsigned int object_id, unsigned int object_instance_id,
                      std::optional<unsigned int> resource_id,
                      std::optional<unsigned int> resoruce_instance_id,
                      std::string value);
};

} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_HPP