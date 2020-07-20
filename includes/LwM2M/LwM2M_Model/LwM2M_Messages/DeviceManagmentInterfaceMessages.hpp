#ifndef __LWM2M_MODEL_DEVICE_MANAGMENT_INTERFACE_MESSAGES_HPP
#define __LWM2M_MODEL_DEVICE_MANAGMENT_INTERFACE_MESSAGES_HPP

#include "LwM2M_Message.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace LwM2M {
struct DeviceManagment_Interface_Message : public Message {
  DeviceManagment_Interface_Message(std::string endpoint_address,
                                    unsigned int endpoint_port,
                                    uint16_t message_id,
                                    std::vector<uint8_t> token,
                                    MessageType message_type)
      : Message(endpoint_address, endpoint_port, message_id, token,
                message_type) {}
};

struct Read_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;

  Read_Request(std::string endpoint_address, unsigned int endpoint_port,
               uint16_t message_id, std::vector<uint8_t> token,
               unsigned int object_id,
               std::optional<unsigned int> object_instance_id,
               std::optional<unsigned int> resource_id,
               std::optional<unsigned int> resoruce_instance_id);
};

struct Discover_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;

  Discover_Request(std::string endpoint_address, unsigned int endpoint_port,
                   uint16_t message_id, std::vector<uint8_t> token,
                   unsigned int object_id,
                   std::optional<unsigned int> object_instance_id,
                   std::optional<unsigned int> resource_id);
};

struct Write_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  unsigned int object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;
  std::string value_;

  Write_Request(std::string endpoint_address, unsigned int endpoint_port,
                uint16_t message_id, std::vector<uint8_t> token,
                unsigned int object_id, unsigned int object_instance_id,
                std::optional<unsigned int> resource_id,
                std::optional<unsigned int> resoruce_instance_id,
                std::string value);
};

struct Write_Attributes_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;
  std::optional<Notify_Attripube> notify_attribute_;

  Write_Attributes_Request(std::string endpoint_address,
                           unsigned int endpoint_port, uint16_t message_id,
                           std::vector<uint8_t> token, unsigned int object_id,
                           std::optional<unsigned int> object_instance_id,
                           std::optional<unsigned int> resource_id,
                           std::optional<unsigned int> resoruce_instance_id,
                           std::optional<Notify_Attripube> notify_attribute);
};

struct Execute_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  unsigned int object_instance_id_;
  unsigned int resource_id_;
  unsigned int resoruce_instance_id_;
  std::optional<std::string> arguments_;

  Execute_Request(std::string endpoint_address, unsigned int endpoint_port,
                  uint16_t message_id, std::vector<uint8_t> token,
                  unsigned int object_id, unsigned int object_instance_id,
                  unsigned int resource_id, unsigned int resoruce_instance_id,
                  std::optional<std::string> arguments);
};

struct Create_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::string value_;

  Create_Request(std::string endpoint_address, unsigned int endpoint_port,
                 uint16_t message_id, std::vector<uint8_t> token,
                 unsigned int object_id, std::string value);
};

struct Delete_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  unsigned int object_instance_id_;

  Delete_Request(std::string endpoint_address, unsigned int endpoint_port,
                 uint16_t message_id, std::vector<uint8_t> token,
                 unsigned int object_id, unsigned int object_instance_id);
};

struct ReadComoposite_Request : public DeviceManagment_Interface_Message {
  ReadComoposite_Request(std::string endpoint_address,
                         unsigned int endpoint_port, uint16_t message_id,
                         std::vector<uint8_t> token);
};

struct WriteComoposite_Request : public DeviceManagment_Interface_Message {
  WriteComoposite_Request(std::string endpoint_address,
                          unsigned int endpoint_port, uint16_t message_id,
                          std::vector<uint8_t> token);
};
} // namespace LwM2M

#endif //__LWM2M_MODEL_DEVICE_MANAGMENT_INTERFACE_MESSAGES_HPP