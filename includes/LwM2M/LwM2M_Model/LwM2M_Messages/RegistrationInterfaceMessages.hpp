#ifndef __LWM2M_MODEL_REGISTRATION_INTERFACE_MESSAGES_HPP
#define __LWM2M_MODEL_REGISTRATION_INTERFACE_MESSAGES_HPP

#include "LwM2M_Message.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace LwM2M_Model {
enum class LwM2M_Version { V1_0, V1_1, UNRECOGNIZED };

enum class BindingType { UDP, TCP, SMS, NON_IP, MALFORMED };

struct Regirstration_Interface_Message : LwM2M_Message {
  Regirstration_Interface_Message(std::string endpoint_address,
                                  unsigned int endpoint_port,
                                  std::vector<uint8_t> token,
                                  MessageType message_type)
      : LwM2M_Message(endpoint_address, endpoint_port, token, message_type) {}
};

struct Register_Request : public Regirstration_Interface_Message {
  std::string endpoint_name_;
  size_t life_time_;
  LwM2M_Version version_;
  BindingType binding_;
  bool queue_mode_;
  std::optional<std::string> sms_number_;
  std::unordered_map<unsigned int, unsigned int> object_instances_map_;

  Register_Request(
      std::string endpoint_address, unsigned int endpoint_port,
      std::vector<uint8_t> token, std::string endpoint_name, size_t life_time,
      LwM2M_Version version, BindingType binding, bool queue_mode,
      std::optional<std::string> sms_number,
      std::unordered_map<unsigned int, unsigned int> object_instances_map);
};

struct Update_Request : public Regirstration_Interface_Message {
  std::string location_;
  std::optional<size_t> lifetime_;
  std::optional<BindingType> binding_;
  std::optional<std::string> sms_number_;
  std::optional<std::unordered_map<unsigned int, unsigned int>>
      object_instances_map_;

  Update_Request(std::string endpoint_address, unsigned int endpoint_port,
                 std::vector<uint8_t> token, std::string location,
                 std::optional<size_t> lifetime,
                 std::optional<BindingType> binding,
                 std::optional<std::string> sms_number,
                 std::optional<std::unordered_map<unsigned int, unsigned int>>
                     object_instances_map);
};

struct Deregister_Request : public Regirstration_Interface_Message {
  std::string location_;

  Deregister_Request(std::string endpoint_address, unsigned int endpoint_port,
                     std::vector<uint8_t> token, std::string location);
};
}; // namespace LwM2M_Model

#endif //__LWM2M_MODEL_REGISTRATION_INTERFACE_MESSAGES_HPP