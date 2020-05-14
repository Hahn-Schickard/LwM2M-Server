#ifndef __LWM2M_MODEL_INFORMATION_REPORTING_INTERFACE_MESSAGES_HPP
#define __LWM2M_MODEL_INFORMATION_REPORTING_INTERFACE_MESSAGES_HPP

#include "LwM2M_Message.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace LwM2M {
class Observe_Request {
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

class Notify_Request {
  std::string token_;
  std::string value_;

public:
  Notify_Request(std::string token, std::string value);
};

class CancelObservation_Request {
  std::string location_;

public:
  CancelObservation_Request(std::string location);
};

class ObserveComposite_Request {
public:
  ObserveComposite_Request() {
    throw UnsupportedOperation("Observe-Composite operation is not supported!");
  }
};

class CancelObserveComposite_Request {
public:
  CancelObserveComposite_Request() {
    throw UnsupportedOperation(
        "Cancel Observe-Composite operation is not supported!");
  }
};

class Send_Request {
public:
  Send_Request() {
    throw UnsupportedOperation("Send operation is not supported!");
  }
};
} // namespace LwM2M

#endif //__LWM2M_MODEL_INFORMATION_REPORTING_INTERFACE_MESSAGES_HPP