#include "LwM2M_Server.hpp"
#include "CoAP_DeviceManagmentMessageDecoder.hpp"
#include "CoAP_Encoder.hpp"
#include "CoAP_RegistrationMessageDecoder.hpp"
#include "CoAP_Server.hpp"
#include "LoggerRepository.hpp"
#include "Threadsafe_Unique_Queue.hpp"

using namespace std;
using namespace HaSLL;

namespace LwM2M {

Server::Server() {}

Server::Server(Configuration config)
    : logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  auto server = make_unique<CoAP::Server>(config.ip_address, config.server_port,
                                          config.read_timeout);
  auto response_handler = make_shared<ResponseHandler>();
  shared_ptr<MessageEncoder> encoder = make_shared<CoAP_Encoder>(
      response_handler, server->getOutgoingMessages());
  registration_ =
      make_shared<DeviceRegistry>(encoder, config.object_descriptors_location);
  tasks_.emplace_back(make_unique<CoAP_RegistrationMessageDecoder>(
                          registration_, server->getIncomingMessages()),
                      "Registration Message Decoder");
  tasks_.emplace_back(make_unique<CoAP_DeviceManagmentMessageDecoder>(
                          response_handler, server->getIncomingMessages()),
                      "Device Managment Message Decoder");
  tasks_.emplace_back(move(server), "CoAP Server");
};

void Server::start() {
  for (auto &stoppabletask : tasks_) {
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
  for (auto &stoppabletask : tasks_) {
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

DevicePtr Server::getDevice(string device_id) {
  return registration_->getDevice(device_id);
}

RegistryEventSourcePtr Server::getEventSource() { return registration_; }

} // namespace LwM2M
