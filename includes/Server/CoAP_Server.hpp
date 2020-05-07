#ifndef __LwM2M_UDP_CONNECTION_HPP
#define __LwM2M_UDP_CONNECTION_HPP

#include "CoAP_Message.hpp"
#include "Logger.hpp"
#include "Threadsafe_Queue.hpp"

#include <chrono>
#include <future>
#include <memory>

namespace LwM2M_Server {

struct CoAP_Server {
  CoAP_Server(bool ip_v6_handler, unsigned int port_id,
              unsigned int task_execution_period);
  ~CoAP_Server();

  void run();

  bool stopRequested() {
    return exitFuture_.wait_for(std::chrono::milliseconds(0)) ==
                   std::future_status::timeout
               ? false
               : true;
  }

  void stop() { exitSignal_.set_value(); }

  std::shared_ptr<CoAP::CoAP_Message> pullRequest();
  void pushResponse(CoAP::CoAP_Message &message);

private:
  bool ip_v6_handler_;
  unsigned int port_id_;
  unsigned int task_execution_period_;
  std::promise<void> exitSignal_;
  std::future<void> exitFuture_;
  std::shared_ptr<ThreadsafeQueue<CoAP::CoAP_Message>> incominng_messages_;
  std::shared_ptr<ThreadsafeQueue<CoAP::CoAP_Message>> outgoing_messages_;
  std::shared_ptr<HaSLL::Logger> logger_;
};
} // namespace LwM2M_Server

#endif //__LwM2M_UDP_CONNECTION_HPP