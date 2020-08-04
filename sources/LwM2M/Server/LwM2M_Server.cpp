#include "LwM2M_Server.hpp"
#include "CoAP_Server.hpp"
#include "CoAP_To_LwM2M.hpp"
#include "LoggerRepository.hpp"
#include "LwM2M_To_CoAP.hpp"
#include "MessageProcessor.hpp"
#include "RegistrationInterface.hpp"
#include "Threadsafe_Queue.hpp"

using namespace std;
using namespace HaSLL;

namespace LwM2M {

struct ProcessIsAlreadyRunning : public runtime_error {
  ProcessIsAlreadyRunning(string const &message) : runtime_error(message) {}
};

struct ProcessIsNotRunning : public runtime_error {
  ProcessIsNotRunning(string const &message) : runtime_error(message) {}
};

Process::Process() : Process(unique_ptr<Stoppable>(), "") {}

Process::Process(unique_ptr<Stoppable> task, string task_name)
    : task_(move(task)), task_thread_(make_unique<thread>()),
      task_name_(task_name) {}

void Process::startTask() {
  if (!task_thread_->joinable())
    task_thread_ = make_unique<thread>([&]() { task_->start(); });
  else {
    string error_msg = "Prcoess" + task_name_ + " is already running";
    throw ProcessIsAlreadyRunning(move(error_msg));
  }
}

void Process::stopTask() {
  if (task_thread_->joinable()) {
    task_->stop();
    task_thread_->join();
  } else {
    string error_msg = "Prcoess" + task_name_ + " is not running";
    throw ProcessIsNotRunning(move(error_msg));
  }
}

string Process::getName() { return task_name_; }

Server::Server() {}

Server::Server(Configuration config)
    : device_registery_(
          make_shared<unordered_map<string, shared_ptr<Device>>>()),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  auto server = make_unique<CoAP::Server>(config.ip_address, config.server_port,
                                          config.read_timeout);
  auto message_queue = make_shared<ThreadsafeQueue<Message>>();
  auto registration = make_shared<RegistrationInterface>(
      device_registery_, config.object_descriptors_location);
  processes_.emplace_back(
      make_unique<MessageProcessor<CoAP::Message>>(
          make_unique<CoAP_To_LwM2M>(message_queue, registration),
          server->getIncomingMessagesQueue(), "IncomingMessageProcessor"),
      "Incoming Message Processor");
  processes_.emplace_back(
      make_unique<MessageProcessor<Message>>(
          make_unique<LwM2M_To_CoAP>(server->getOutgoingMessagesQueue()),
          message_queue, "OutgoingMessageProcessor"),
      "Outgoing Message Processor");
  processes_.emplace_back(move(server), "CoAP Server");
};

void Server::run() {
  for (auto &process : processes_) {
    try {
      process.startTask();
    } catch (ProcessIsAlreadyRunning &ex) {
      logger_->log(SeverityLevel::TRACE, ex.what());
      continue;
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR, ex.what());
      break;
    }
  }
}

void Server::stop() {
  for (auto &process : processes_) {
    try {
      process.stopTask();
    } catch (ProcessIsNotRunning &ex) {
      logger_->log(SeverityLevel::TRACE, ex.what());
      continue;
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR, ex.what());
      break;
    }
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
