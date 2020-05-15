#ifndef __LWM2M_MODEL_INFORMATION_REPORTING_INTERFACE_MESSAGES_HPP
#define __LWM2M_MODEL_INFORMATION_REPORTING_INTERFACE_MESSAGES_HPP

#include "LwM2M_Message.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace LwM2M_Model {
class InformationReporting_Interface_Message : public LwM2M_Message {
public:
  InformationReporting_Interface_Message(MessageType message_type)
      : LwM2M_Message(InterfaceType::INFORMATION_REPORTING, message_type) {}
};

class Observe_Request : public InformationReporting_Interface_Message {
  unsigned int object_id_;
  std::optional<unsigned int> object_instance_id_;
  std::optional<unsigned int> resource_id_;
  std::optional<unsigned int> resoruce_instance_id_;

public:
  Observe_Request(unsigned int object_id,
                  std::optional<unsigned int> object_instance_id,
                  std::optional<unsigned int> resource_id,
                  std::optional<unsigned int> resoruce_instance_id);
};

class Notify_Request : public InformationReporting_Interface_Message {
  std::string token_;
  std::string value_;

public:
  Notify_Request(std::string token, std::string value);
};

class CancelObservation_Request
    : public InformationReporting_Interface_Message {
  std::string location_;

public:
  CancelObservation_Request(std::string location);
};

class ObserveComposite_Request : public InformationReporting_Interface_Message {
public:
  ObserveComposite_Request();
};

class CancelObserveComposite_Request
    : public InformationReporting_Interface_Message {
public:
  CancelObserveComposite_Request();
};

class Send_Request : public InformationReporting_Interface_Message {
public:
  Send_Request();
};
} // namespace LwM2M_Model

#endif //__LWM2M_MODEL_INFORMATION_REPORTING_INTERFACE_MESSAGES_HPP