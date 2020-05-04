#ifndef __LwM2M_UDP_CONNECTION_HPP
#define __LwM2M_UDP_CONNECTION_HPP

#include <chrono>
#include <future>

namespace LwM2M_Server {

struct UDPConnection {
  UDPConnection(bool ip_v6_handler, unsigned int port_id,
                unsigned int task_execution_period)
      : ip_v6_handler_(ip_v6_handler), port_id_(port_id),
        task_execution_period_(task_execution_period),
        exitFuture_(exitSignal_.get_future()){};
  void run();

  bool stopRequested() {
    return exitFuture_.wait_for(std::chrono::milliseconds(0)) ==
                   std::future_status::timeout
               ? false
               : true;
  }

  void stop() { exitSignal_.set_value(); }

private:
  bool ip_v6_handler_;
  unsigned int port_id_;
  unsigned int task_execution_period_;
  std::promise<void> exitSignal_;
  std::future<void> exitFuture_;
};
} // namespace LwM2M_Server

#endif //__LwM2M_UDP_CONNECTION_HPP