#ifndef __LWM2M_MODEL_DEVICE_MANAGMENT_INTERFACE_MESSAGES_HPP
#define __LWM2M_MODEL_DEVICE_MANAGMENT_INTERFACE_MESSAGES_HPP

#include "LwM2M_Message.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace LwM2M_Model {
class Read_Request {
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

class Discover_Request {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;

public:
  Discover_Request(unsigned int object_id,
                   std::optional<unsigned int> object_instance_id,
                   std::optional<unsigned int> resource_id);
};

class Write_Request {
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

class Write_Attributes_Request {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;
  std::optional<unsigned int> minimum_period_;
  std::optional<unsigned int> maximum_period_;
  std::optional<unsigned int> greater_than_;
  std::optional<unsigned int> less_than_;
  std::optional<unsigned int> step_;
  std::optional<unsigned int> minimum_evaluation_period_;
  std::optional<unsigned int> maximum_evaluation_period_;

public:
  Write_Attributes_Request(
      unsigned int object_id, std::optional<unsigned int> object_instance_id,
      std::optional<unsigned int> resource_id,
      std::optional<unsigned int> resoruce_instance_id,
      std::optional<unsigned> int minimum_period,
      std::optional<unsigned> int maximum_period,
      std::optional<unsigned> int greater_than,
      std::optional<unsigned int> less_than, std::optional<unsigned> int step,
      std::optional<unsigned> int minimum_evaluation_period,
      std::optional<unsigned> int maximum_evaluation_period);
};

class Execute_Request {
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

class Create_Request {
  unsigned int object_id_;
  std::string value_;

public:
  Create_Request(unsigned int object_id, std::string value);
};

class Delete_Request {
  unsigned int object_id_;
  unsigned int object_instance_id_;

public:
  Delete_Request(unsigned int object_id, unsigned int object_instance_id);
};

class ReadComoposite_Request {
public:
  ReadComoposite_Request() {
    throw UnsupportedOperation("Read-Composite operation is not supported!");
  }
};

class WriteComoposite_Request {
public:
  WriteComoposite_Request() {
    throw UnsupportedOperation("Write-Composite operation is not supported!");
  }
};
} // namespace LwM2M_Model

#endif //__LWM2M_MODEL_DEVICE_MANAGMENT_INTERFACE_MESSAGES_HPP