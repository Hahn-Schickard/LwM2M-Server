#include "LwM2M_Server.hpp"

using namespace std;
using namespace CoAP;

namespace LwM2M_Model {

LwM2M_Server::LwM2M_Server(LwM2M_Configuration config)
    : server_(config.ipv6_flag, config.server_port, config.read_timeout),
      lwm2m_message_queue_(make_shared<ThreadsafeQueue<LwM2M_Message>>()),
      message_processing_(server_.getIncomingMessagesQueue(),
                          lwm2m_message_queue_,
                          server_.getOutgoingMessagesQueue()),
      message_sorter_(lwm2m_message_queue_),
      registration_(lwm2m_message_queue_,
                    message_sorter_.getRegistrationInterfaceQueue(),
                    config.object_descriptors_location) {
  processes_ = {new thread([&]() { server_.run(); }),
                new thread([&]() { message_sorter_.run(); }),
                new thread([&]() { registration_.run(); })};
};

LwM2M_Server::~LwM2M_Server() {
  registration_.stop();
  message_sorter_.stop();
  server_.stop();
  for (auto process : processes_) {
    process->join();
    delete process;
  }
}

} // namespace LwM2M_Model
