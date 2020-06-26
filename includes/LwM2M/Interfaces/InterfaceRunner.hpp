#ifndef __LWM2M_INTERFACE_RUNNER_HPP
#define __LWM2M_INTERFACE_RUNNER_HPP

#include "LwM2M_Message.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_Queue.hpp"

#include <memory>
#include <thread>

namespace LwM2M_Model {
class InterfaceRunner : public Stoppable {
protected:
  std::shared_ptr<ThreadsafeQueue<LwM2M_Message>> outgoing_message_queue_;

public:
  InterfaceRunner()
      : outgoing_message_queue_(
            std::shared_ptr<ThreadsafeQueue<LwM2M_Message>>()) {}
  InterfaceRunner(
      std::shared_ptr<ThreadsafeQueue<LwM2M_Message>> outgoing_message_queue)
      : outgoing_message_queue_(outgoing_message_queue) {}
  ~InterfaceRunner() = default;
};
} // namespace LwM2M_Model

#endif //__LWM2M_INTERFACE_RUNNER_HPP