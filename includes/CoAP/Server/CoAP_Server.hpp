#ifndef __COAP_SERVER_HPP
#define __COAP_SERVER_HPP

#include "CoAP_Message.hpp"
#include "Logger.hpp"
#include "Stoppable.hpp"
#include "Threadsafe_Queue.hpp"

#include <memory>
#include <string>

namespace CoAP {

class SocketInterface {
public:
  virtual ~SocketInterface() = default;

  virtual void listen() = 0;
};

class Server : public Stoppable {
  std::shared_ptr<ThreadsafeQueue<Message>> incominng_messages_;
  std::shared_ptr<ThreadsafeQueue<Message>> outgoing_messages_;
  std::unique_ptr<SocketInterface> socket_;
  std::shared_ptr<HaSLL::Logger> logger_;

public:
  Server();
  Server(const std::string &ip_address, unsigned int port_id,
         unsigned int task_execution_period);
  Server(bool ip_v6_handler, unsigned int port_id,
         unsigned int task_execution_period);
  ~Server();

  void run();

  std::unique_ptr<Message> pullRequest();
  void pushResponse(std::unique_ptr<Message> message);

  std::shared_ptr<ThreadsafeQueue<Message>> getIncomingMessagesQueue();
  std::shared_ptr<ThreadsafeQueue<Message>> getOutgoingMessagesQueue();
};
} // namespace CoAP

#endif //__COAP_SERVER_HPP