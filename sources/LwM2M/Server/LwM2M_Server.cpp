#include "LwM2M_Server.hpp"
#include "CoAP_Server.hpp"
#include "CoAP_To_LwM2M.hpp"
#include "LwM2M_To_CoAP.hpp"
#include "MessageProcessor.hpp"
#include "MessageSorter.hpp"
#include "RegistrationInterface.hpp"
#include "Threadsafe_Queue.hpp"

using namespace std;
using namespace CoAP;

namespace LwM2M_Model {

LwM2M_Server::LwM2M_Server(LwM2M_Configuration config) {
  auto server = make_unique<CoAP_Server>(config.ip_address, config.server_port,
                                         config.read_timeout);
  processes_.emplace_back(make_unique<MessageProcessor<CoAP::CoAP_Message>>(
      make_unique<CoAP_To_LwM2M>(), server->getIncomingMessagesQueue(),
      "IncomingMessageProcessor"));
  auto message_queue = make_shared<ThreadsafeQueue<LwM2M_Message>>();
  processes_.emplace_back(make_unique<MessageProcessor<LwM2M_Message>>(
      make_unique<LwM2M_To_CoAP>(server->getOutgoingMessagesQueue()),
      message_queue, "OutgoingMessageProcessor"));
  auto sorter = make_unique<MessageSorter>(message_queue);
  processes_.emplace_back(make_unique<RegistrationInterface>(
      message_queue, sorter->getRegistrationInterfaceQueue(),
      config.object_descriptors_location));
  processes_.emplace_back(move(sorter));
  processes_.emplace_back(move(server));
};

void LwM2M_Server::run() {
  for (const auto &process : processes_) {
    process_threads_.emplace_back(
        make_unique<thread>([&]() { process->start(); }));
  }
}

void LwM2M_Server::stop() {
  stop();
  for (const auto &process : processes_) {
    process->stop();
  }
  for (const auto &process_thread : process_threads_) {
    process_thread->join();
  }
}

} // namespace LwM2M_Model
