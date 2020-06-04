#include "CoAP_Server.hpp"
#include "LoggerRepository.hpp"

#include <asio.hpp>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#define PACKET_SIZE_UPPER_BOUND 1024

using namespace std;
using namespace HaSLL;
using namespace CoAP;
using namespace asio;
using asio::ip::udp;
namespace LwM2M_Server {

udp selectProtocol(bool ip_v6) {
  if (ip_v6) {
    return asio::ip::udp::v6();
  } else {
    return asio::ip::udp::v4();
  }
}

void handleReturnCode(string failure_message, asio::error_code return_code) {
  if (return_code) {
    string error_msg = failure_message + " " + return_code.message();
    throw Network_IO_Exception(error_msg);
  }
}

class CoAP_Port {
  io_context io_context_;
  udp::socket socket_;
  unsigned int task_execution_period_;
  shared_ptr<ThreadsafeQueue<CoAP_Message>> incominng_messages_;
  shared_ptr<ThreadsafeQueue<CoAP_Message>> outgoing_messages_;
  shared_ptr<Logger> logger_;

  void receive() {
    vector<uint8_t> udp_datagram(PACKET_SIZE_UPPER_BOUND);
    udp::endpoint remote_endpoint;
    asio::error_code return_code;

    io_context_.restart();
    future<size_t> datagram_future = socket_.async_receive_from(
        asio::buffer(udp_datagram, PACKET_SIZE_UPPER_BOUND), remote_endpoint,
        asio::use_future);
    if (!io_context_.stopped()) {
      io_context_.run();

      if (datagram_future.wait_for(asio::chrono::seconds(
              task_execution_period_)) == future_status::ready &&
          !udp_datagram.empty()) {

        CoAP_Message message(remote_endpoint.address().to_string(),
                             remote_endpoint.port(), move(udp_datagram));
        incominng_messages_->push(message);
      }
    }
  }

  void send(shared_ptr<CoAP_Message> message) {
    asio::error_code return_code;
    socket_.send_to(
        asio::buffer(message->toPacket()),
        udp::endpoint(asio::ip::address::from_string(message->getReceiverIP()),
                      message->getReceiverPort()),
        0, return_code);
    handleReturnCode("Could not send " +
                         toString(message->getHeader().getMesageType()) + " " +
                         toString(message->getHeader().getCodeType()) + " to " +
                         message->getReceiverIP() + ":" +
                         to_string(message->getReceiverPort()),
                     return_code);
  }

public:
  CoAP_Port(bool ip_v6_handler, unsigned int port_id,
            unsigned int task_execution_period,
            shared_ptr<ThreadsafeQueue<CoAP_Message>> incominng_messages,
            shared_ptr<ThreadsafeQueue<CoAP_Message>> outgoing_messages)
      : io_context_(),
        socket_(io_context_,
                udp::endpoint(selectProtocol(ip_v6_handler), port_id)),
        task_execution_period_(task_execution_period),
        incominng_messages_(incominng_messages),
        outgoing_messages_(outgoing_messages),
        logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
    logger_->log(SeverityLevel::TRACE,
                 "CoAP_Port::CoAP_Port({},{},{}, incominng_messages_buffer, "
                 "outgoing_messages_buffer)",
                 ip_v6_handler, port_id, task_execution_period);
    logger_->log(SeverityLevel::INFO, "Opening a {} port on {}",
                 socket_.local_endpoint().port(),
                 socket_.local_endpoint().address().to_string());
  }

  ~CoAP_Port() {
    logger_->log(SeverityLevel::TRACE, "~CoAP_Port::CoAP_Port()");
    LoggerRepository::getInstance().deregisterLoger(logger_->getName());
  }

  void listen() {
    shared_ptr<CoAP_Message> message = outgoing_messages_->try_pop();
    if (message) {
      send(move(message));
    } else {
      receive();
    }
  }
}; // namespace LwM2M_Server

CoAP_Server::CoAP_Server(bool ip_v6_handler, unsigned int port_id,
                         unsigned int task_execution_period)
    : ip_v6_handler_(ip_v6_handler), port_id_(port_id),
      task_execution_period_(task_execution_period),
      incominng_messages_(make_shared<ThreadsafeQueue<CoAP_Message>>()),
      outgoing_messages_(make_shared<ThreadsafeQueue<CoAP_Message>>()),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  logger_->log(SeverityLevel::TRACE, "CoAP_Server::CoAP_Server({},{},{})",
               ip_v6_handler, port_id, task_execution_period);
};

CoAP_Server::~CoAP_Server() {
  logger_->log(SeverityLevel::TRACE, "~CoAP_Server::CoAP_Server()");
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

void CoAP_Server::run() {
  logger_->log(SeverityLevel::TRACE, "CoAP_Server::run()");
  CoAP_Port port(ip_v6_handler_, port_id_, task_execution_period_,
                 incominng_messages_, outgoing_messages_);
  do {
    try {
      port.listen();
    } catch (Network_IO_Exception &ex) {
      logger_->log(SeverityLevel::ERROR, ex.what());
    }
  } while (!stopRequested());
}

shared_ptr<CoAP_Message> CoAP_Server::pullRequest() {
  logger_->log(SeverityLevel::TRACE, "CoAP_Server::pullRequest()");
  return incominng_messages_->wait_and_pop();
}

void CoAP_Server::pushResponse(CoAP_Message message) {
  logger_->log(SeverityLevel::TRACE, "CoAP_Server::pushResponse()");
  outgoing_messages_->push(message);
}

shared_ptr<ThreadsafeQueue<CoAP_Message>>
CoAP_Server::getIncomingMessagesQueue() {
  logger_->log(SeverityLevel::TRACE, "CoAP_Server::getIncomingMessagesQueue()");
  return incominng_messages_;
}

shared_ptr<ThreadsafeQueue<CoAP_Message>>
CoAP_Server::getOutgoingMessagesQueue() {
  logger_->log(SeverityLevel::TRACE, "CoAP_Server::getOutgoingMessagesQueue()");
  return outgoing_messages_;
}
} // namespace LwM2M_Server