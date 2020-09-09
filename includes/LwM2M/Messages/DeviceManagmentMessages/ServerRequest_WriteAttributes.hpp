#ifndef __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_ATTRIBUTE_HPP
#define __LWM2M_DEVICE_MANAGMENT_SERVER_REQUEST_WRITE_ATTRIBUTE_HPP

#include "DeviceManagmentMessage.hpp"

namespace LwM2M {

struct Notify_Attribute {
  std::optional<unsigned int> minimum_period_;
  std::optional<unsigned int> maximum_period_;
  std::optional<unsigned int> greater_than_;
  std::optional<unsigned int> less_than_;
  std::optional<unsigned int> step_;
  std::optional<unsigned int> minimum_evaluation_period_;
  std::optional<unsigned int> maximum_evaluation_period_;

  Notify_Attribute(std::optional<unsigned int> minimum_period,
                   std::optional<unsigned int> maximum_period,
                   std::optional<unsigned int> greater_than,
                   std::optional<unsigned int> less_than,
                   std::optional<unsigned int> step,
                   std::optional<unsigned int> minimum_evaluation_period,
                   std::optional<unsigned int> maximum_evaluation_period);
};

struct ServerRequest_WriteAttributes : public DeviceManagmentMessage {
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