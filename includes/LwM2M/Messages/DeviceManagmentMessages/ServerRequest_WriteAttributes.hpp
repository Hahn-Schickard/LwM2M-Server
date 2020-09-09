#ifndef __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_ATTRIBUTE_HPP
#define __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_ATTRIBUTE_HPP

#include "DeviceManagmentInterfaceMessage.hpp"

namespace LwM2M {
struct ServerRequest_WriteAttributes
    : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;
  std::optional<Notify_Attribute> notify_attribute_;

  ServerRequest_WriteAttributes(
      std::string endpoint_address, unsigned int endpoint_port,
      unsigned int object_id, std::optional<unsigned int> object_instance_id,
      std::optional<unsigned int> resource_id,
      std::optional<unsigned int> resoruce_instance_id,
      std::optional<Notify_Attribute> notify_attribute);
};
} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_ATTRIBUTE_HPP