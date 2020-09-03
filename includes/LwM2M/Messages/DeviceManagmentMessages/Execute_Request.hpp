#ifndef __LWM2M_DEVICE_MANAGMENT_EXECUTE_REQUEST_HPP
#define __LWM2M_DEVICE_MANAGMENT_EXECUTE_REQUEST_HPP

#include "DeviceManagmentInterfaceMessage.hpp"

namespace LwM2M {
struct Execute_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  unsigned int object_instance_id_;
  unsigned int resource_id_;
  unsigned int resoruce_instance_id_;
  std::optional<std::string> arguments_;

  Execute_Request(std::string endpoint_address, unsigned int endpoint_port,
                  unsigned int object_id, unsigned int object_instance_id,
                  unsigned int resource_id, unsigned int resoruce_instance_id,
                  std::optional<std::string> arguments);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_EXECUTE_REQUEST_HPP