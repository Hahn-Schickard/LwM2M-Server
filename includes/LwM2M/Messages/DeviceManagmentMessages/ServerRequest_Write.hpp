#ifndef __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_HPP
#define __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_HPP

#include "DeviceManagmentMessage.hpp"
#include "LwM2M_DataFormat.hpp"

namespace LwM2M {
struct ServerRequest_Write : public DeviceManagmentMessage {
  unsigned int object_id_;
  unsigned int object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;
  std::shared_ptr<DataFormat> payload_;

  ServerRequest_Write(std::string endpoint_address, unsigned int endpoint_port,
                      unsigned int object_id, unsigned int object_instance_id,
                      std::optional<unsigned int> resource_id,
                      std::optional<unsigned int> resoruce_instance_id,
                      std::shared_ptr<DataFormat> payload);
};

} // namespace LwM2M

#endif //__LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_HPP