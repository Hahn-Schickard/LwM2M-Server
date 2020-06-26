#ifndef __COAP_SERVER_HPP
#define __COAP_SERVER_HPP

#include "CoAP_Message.hpp"
#include "Logger.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_Queue.hpp"

#include <memory>

namespace CoAP {

class CoAP_Server : public Stoppable {
  bool ip_v6_handler_;
  unsigned int port_id_;
  unsigned int task_execution_period_;
  std::shared_ptr<ThreadsafeQueue<CoAP_Message>> incominng_messages_;
  std::shared_ptr<ThreadsafeQueue<CoAP_Message>> outgoing_messages_;
  std::shared_ptr<HaSLL::Logger> logger_;

public:
  CoAP_Server(bool ip_v6_handler, unsigned int port_id,
              unsigned int task_execution_period);
  ~CoAP_Server();

  void run();

  std::shared_ptr<CoAP_Message> pullRequest();
  void pushResponse(CoAP_Message message);

  std::shared_ptr<ThreadsafeQueue<CoAP_Message>> getIncomingMessagesQueue();
  std::shared_ptr<ThreadsafeQueue<CoAP_Message>> getOutgoingMessagesQueue();
};
} // namespace CoAP

#endif //__COAP_SERVER_HPP