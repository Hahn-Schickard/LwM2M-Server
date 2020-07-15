#include "LwM2M_Server.hpp"
#include "CoAP_Server.hpp"
#include "CoAP_To_LwM2M.hpp"
#include "LwM2M_To_CoAP.hpp"
#include "MessageProcessor.hpp"
#include "MessageSorter.hpp"
#include "RegistrationInterface.hpp"
#include "Threadsafe_Queue.hpp"

using namespace std;

namespace LwM2M {

Server::Server() {}

Server::Server(Configuration config)
    : device_registery_(
          make_shared<unordered_map<string, shared_ptr<Device>>>()) {
  auto server = make_unique<CoAP::Server>(config.ip_address, config.server_port,
                                          config.read_timeout);
  auto message_queue = make_shared<ThreadsafeQueue<Message>>();
  processes_.emplace_back(make_unique<MessageProcessor<CoAP::Message>>(
      make_unique<CoAP_To_LwM2M>(message_queue),
      server->getIncomingMessagesQueue(), "IncomingMessageProcessor"));
  processes_.emplace_back(make_unique<MessageProcessor<Message>>(
      make_unique<LwM2M_To_CoAP>(server->getOutgoingMessagesQueue()),
      message_queue, "OutgoingMessageProcessor"));
  auto sorter = make_unique<MessageSorter>(message_queue);
  processes_.emplace_back(make_unique<RegistrationInterface>(
      message_queue, sorter->getRegistrationInterfaceQueue(), device_registery_,
      config.object_descriptors_location));
  processes_.push_back(move(sorter));
  processes_.push_back(move(server));
};

void Server::run() {
  for (const auto &process : processes_) {
    process_threads_.emplace_back(
        make_unique<thread>([&]() { process->start(); }));
  }
}

void Server::stop() {
  stop();
  for (const auto &process : processes_) {
    process->stop();
  }
  for (const auto &process_thread : process_threads_) {
    process_thread->join();
  }
}

shared_ptr<Device> Server::getDevice(string device_id) {
  auto it = device_registery_->find(device_id);
  shared_ptr<Device> result;
  if (it != device_registery_->end())
    result = it->second;
  return result;
}

} // namespace LwM2M
