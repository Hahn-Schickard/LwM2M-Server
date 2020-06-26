#ifndef __LWM2M_MODEL_INFORMATION_REPORTING_INTERFACE_MESSAGES_HPP
#define __LWM2M_MODEL_INFORMATION_REPORTING_INTERFACE_MESSAGES_HPP

#include "LwM2M_Message.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace LwM2M_Model {
struct InformationReporting_Interface_Message : public LwM2M_Message {
  InformationReporting_Interface_Message(std::string endpoint_address,
                                         unsigned int endpoint_port,
                                         std::vector<uint8_t> token,
                                         MessageType message_type)
      : LwM2M_Message(endpoint_address, endpoint_port, token, message_type) {}
};

struct Observe_Request : public InformationReporting_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;

  Observe_Request(std::string endpoint_address, unsigned int endpoint_port,
                  std::vector<uint8_t> token, unsigned int object_id,
                  std::optional<unsigned int> object_instance_id,
                  std::optional<unsigned int> resource_id,
                  std::optional<unsigned int> resoruce_instance_id);
};

struct Notify_Request : public InformationReporting_Interface_Message {
  std::string value_;

  Notify_Request(std::string endpoint_address, unsigned int endpoint_port,
                 std::vector<uint8_t> token, std::string value);
};

struct CancelObservation_Request
    : public InformationReporting_Interface_Message {
  std::string location_;

  CancelObservation_Request(std::string endpoint_address,
                            unsigned int endpoint_port,
                            std::vector<uint8_t> token, std::string location);
};

struct ObserveComposite_Request
    : public InformationReporting_Interface_Message {
  ObserveComposite_Request(std::string endpoint_address,
                           unsigned int endpoint_port,
                           std::vector<uint8_t> token);
};

struct CancelObserveComposite_Request
    : public InformationReporting_Interface_Message {
  CancelObserveComposite_Request(std::string endpoint_address,
                                 unsigned int endpoint_port,
                                 std::vector<uint8_t> token);
};

struct Send_Request : public InformationReporting_Interface_Message {
  Send_Request(std::string endpoint_address, unsigned int endpoint_port,
               std::vector<uint8_t> token);
};
} // namespace LwM2M_Model

#endif //__LWM2M_MODEL_INFORMATION_REPORTING_INTERFACE_MESSAGES_HPP