#ifndef __LWM2M_SERVER_HPP
#define __LWM2M_SERVER_HPP

#include "CoAP_Server.hpp"
#include "MessageProcessorManager.hpp"
#include "MessageSorter.hpp"
#include "RegistrationInterface.hpp"
#include "Threadsafe_Queue.hpp"

#include <memory>
#include <thread>

namespace LwM2M_Model {

struct LwM2M_Configuration {
  std::string object_descriptors_location;
  std::string ip_address;
  unsigned int server_port;
  unsigned int read_timeout;
};

class LwM2M_Server {
  CoAP::CoAP_Server server_;
  std::shared_ptr<ThreadsafeQueue<LwM2M_Message>> lwm2m_message_queue_;
  MessageProcessorManager message_processing_;
  MessageSorter message_sorter_;
  RegistrationInterface registration_;
  std::vector<std::thread *> processes_;

public:
  LwM2M_Server(LwM2M_Configuration config);
  ~LwM2M_Server();
};
} // namespace LwM2M_Model
#endif //__LWM2M_SERVER_HPP