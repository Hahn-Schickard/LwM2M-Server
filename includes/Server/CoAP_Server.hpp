#ifndef __LwM2M_UDP_CONNECTION_HPP
#define __LwM2M_UDP_CONNECTION_HPP

#include "CoAP_Message.hpp"
#include "Logger.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_Queue.hpp"

#include <memory>

namespace LwM2M_Server {

struct CoAP_Server : public Stoppable {
  CoAP_Server(bool ip_v6_handler, unsigned int port_id,
              unsigned int task_execution_period);
  ~CoAP_Server();

  void run();

  std::shared_ptr<CoAP::CoAP_Message> pullRequest();
  void pushResponse(CoAP::CoAP_Message message);

  std::shared_ptr<ThreadsafeQueue<CoAP::CoAP_Message>>
  getIncomingMessagesQueue();
  std::shared_ptr<ThreadsafeQueue<CoAP::CoAP_Message>>
  getOutgoingMessagesQueue();

private:
  bool ip_v6_handler_;
  unsigned int port_id_;
  unsigned int task_execution_period_;
  std::shared_ptr<ThreadsafeQueue<CoAP::CoAP_Message>> incominng_messages_;
  std::shared_ptr<ThreadsafeQueue<CoAP::CoAP_Message>> outgoing_messages_;
  std::shared_ptr<HaSLL::Logger> logger_;
};
} // namespace LwM2M_Server

#endif //__LwM2M_UDP_CONNECTION_HPP