#ifndef __LWM2M_INTERFACE_RUNNER_HPP
#define __LWM2M_INTERFACE_RUNNER_HPP

#include "LwM2M_Message.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_Queue.hpp"

#include <memory>
#include <thread>

namespace LwM2M {
class InterfaceRunner : public Stoppable {
protected:
  std::shared_ptr<ThreadsafeQueue<Message>> outgoing_message_queue_;

public:
  InterfaceRunner()
      : outgoing_message_queue_(std::shared_ptr<ThreadsafeQueue<Message>>()) {}
  InterfaceRunner(
      std::shared_ptr<ThreadsafeQueue<Message>> outgoing_message_queue)
      : outgoing_message_queue_(outgoing_message_queue) {}
  ~InterfaceRunner() = default;
};
} // namespace LwM2M

#endif //__LWM2M_INTERFACE_RUNNER_HPP