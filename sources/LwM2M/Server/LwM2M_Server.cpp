#include "LwM2M_Server.hpp"
#include "CoAP_Decoder.hpp"
#include "CoAP_Encoder.hpp"
#include "CoAP_Server.hpp"
#include "LoggerRepository.hpp"
#include "Threadsafe_Unique_Queue.hpp"

using namespace std;
using namespace HaSLL;

namespace LwM2M {

struct StoppableTaskIsAlreadyRunning : public runtime_error {
  StoppableTaskIsAlreadyRunning(string const &message)
      : runtime_error(message) {}
};

struct StoppableTaskIsNotRunning : public runtime_error {
  StoppableTaskIsNotRunning(string const &message) : runtime_error(message) {}
};

StoppableTask::StoppableTask() : StoppableTask(unique_ptr<Stoppable>(), "") {}

StoppableTask::StoppableTask(unique_ptr<Stoppable> task, string task_name)
    : task_(move(task)), task_thread_(make_unique<thread>()),
      task_name_(task_name) {}

void StoppableTask::startTask() {
  if (!task_thread_->joinable())
    task_thread_ = make_unique<thread>([&]() { task_->start(); });
  else {
    string error_msg = "Task" + task_name_ + " is already running";
    throw StoppableTaskIsAlreadyRunning(move(error_msg));
  }
}

void StoppableTask::stopTask() {
  if (task_thread_->joinable()) {
    task_->stop();
    task_thread_->join();
  } else {
    string error_msg = "Task" + task_name_ + " is not running";
    throw StoppableTaskIsNotRunning(move(error_msg));
  }
}

string StoppableTask::getName() { return task_name_; }

Server::Server() {}

Server::Server(Configuration config)
    : device_registery_(
          make_shared<unordered_map<string, shared_ptr<Device>>>()),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  auto server = make_unique<CoAP::Server>(config.ip_address, config.server_port,
                                          config.read_timeout);
  auto response_handler = make_shared<ResponseHandler>();
  shared_ptr<MessageEncoder> encoder = make_shared<CoAP_Encoder>(
      response_handler, server->getOutgoingMessagesQueue());
  registration_ = make_shared<RegistryHandler>(
      device_registery_, encoder, config.object_descriptors_location);
  stoppabletaskes_.emplace_back(
      make_unique<CoAP_Decoder>(
          registration_, server->getIncomingMessagesQueue(), response_handler),
      "Incoming Message StoppableTaskor");
  stoppabletaskes_.emplace_back(move(server), "CoAP Server");
};

void Server::start() {
  for (auto &stoppabletask : stoppabletaskes_) {
    try {
      stoppabletask.startTask();
    } catch (StoppableTaskIsAlreadyRunning &ex) {
      logger_->log(SeverityLevel::TRACE, ex.what());
      continue;
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR, ex.what());
      break;
    }
  }
}

void Server::stop() {
  for (auto &stoppabletask : stoppabletaskes_) {
    try {
      stoppabletask.stopTask();
    } catch (StoppableTaskIsNotRunning &ex) {
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

RegistryEventSourcePtr Server::getEventSource() { return registration_; }

} // namespace LwM2M
