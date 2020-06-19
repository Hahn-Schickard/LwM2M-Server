#ifndef __LWM2M_MODEL_DEVICE_MANAGMENT_INTERFACE_MESSAGES_HPP
#define __LWM2M_MODEL_DEVICE_MANAGMENT_INTERFACE_MESSAGES_HPP

#include "LwM2M_Message.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace LwM2M_Model {
class DeviceManagment_Interface_Message : public LwM2M_Message {
public:
  DeviceManagment_Interface_Message(MessageType message_type)
      : LwM2M_Message(InterfaceType::DEVICE_MANAGMENT, message_type) {}
};

class Read_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;

public:
  Read_Request(unsigned int object_id,
               std::optional<unsigned int> object_instance_id,
               std::optional<unsigned int> resource_id,
               std::optional<unsigned int> resoruce_instance_id);
};

class Discover_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;

public:
  Discover_Request(unsigned int object_id,
                   std::optional<unsigned int> object_instance_id,
                   std::optional<unsigned int> resource_id);
};

class Write_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  unsigned int object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;
  std::string value_;

public:
  Write_Request(unsigned int object_id, unsigned int object_instance_id,
                std::optional<unsigned int> resource_id,
                std::optional<unsigned int> resoruce_instance_id,
                std::string value);
};

class Write_Attributes_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;
  std::optional<Notify_Attripube> notify_attribute_;

public:
  Write_Attributes_Request(unsigned int object_id,
                           std::optional<unsigned int> object_instance_id,
                           std::optional<unsigned int> resource_id,
                           std::optional<unsigned int> resoruce_instance_id,
                           std::optional<Notify_Attripube> notify_attribute);
};

class Execute_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  unsigned int object_instance_id_;
  unsigned int resource_id_;
  unsigned int resoruce_instance_id_;
  std::optional<std::string> arguments_;

public:
  Execute_Request(unsigned int object_id, unsigned int object_instance_id,
                  unsigned int resource_id, unsigned int resoruce_instance_id,
                  std::optional<std::string> arguments);
};

class Create_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  std::string value_;

public:
  Create_Request(unsigned int object_id, std::string value);
};

class Delete_Request : public DeviceManagment_Interface_Message {
  unsigned int object_id_;
  unsigned int object_instance_id_;

public:
  Delete_Request(unsigned int object_id, unsigned int object_instance_id);
};

class ReadComoposite_Request : public DeviceManagment_Interface_Message {
public:
  ReadComoposite_Request();
};

class WriteComoposite_Request : public DeviceManagment_Interface_Message {
public:
  WriteComoposite_Request();
};
} // namespace LwM2M_Model

#endif //__LWM2M_MODEL_DEVICE_MANAGMENT_INTERFACE_MESSAGES_HPP