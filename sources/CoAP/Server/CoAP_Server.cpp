#include "CoAP_Server.hpp"
#include "LoggerRepository.hpp"

#include <asio.hpp>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#define PACKET_SIZE_UPPER_BOUND 65536

using namespace std;
using namespace HaSLL;
using namespace asio;
using asio::ip::udp;
namespace CoAP {

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

class Socket : public SocketInterface {
  io_context io_context_;
  udp::socket socket_;
  unsigned int task_execution_period_;
  shared_ptr<ThreadsafeQueue<Message>> incominng_messages_;
  shared_ptr<ThreadsafeQueue<Message>> outgoing_messages_;
  shared_ptr<Logger> logger_;

  void receive() {
    vector<uint8_t> udp_datagram(PACKET_SIZE_UPPER_BOUND);
    udp::endpoint remote_endpoint;
    asio::error_code return_code;

    io_context_.restart();
    future<size_t> bytes_read = socket_.async_receive_from(
        asio::buffer(udp_datagram), remote_endpoint, asio::use_future);
    if (!io_context_.stopped()) {
      io_context_.run_for(asio::chrono::seconds(task_execution_period_));

      if (bytes_read.wait_for(asio::chrono::seconds(task_execution_period_)) ==
              future_status::ready &&
          !udp_datagram.empty()) {
        try {
          udp_datagram.resize(bytes_read.get());
          auto message =
              make_unique<Message>(remote_endpoint.address().to_string(),
                                   remote_endpoint.port(), move(udp_datagram));
          incominng_messages_->push(move(message));
        } catch (const domain_error &ex) {
          logger_->log(SeverityLevel::ERROR, ex.what());
        }
      }
    }
  }

  void send(unique_ptr<Message> message) {
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
  Socket(udp::endpoint socket_endpoint, unsigned int task_execution_period,
         shared_ptr<ThreadsafeQueue<Message>> incominng_messages,
         shared_ptr<ThreadsafeQueue<Message>> outgoing_messages)
      : io_context_(), socket_(io_context_, socket_endpoint),
        task_execution_period_(task_execution_period),
        incominng_messages_(incominng_messages),
        outgoing_messages_(outgoing_messages),
        logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
    logger_->log(SeverityLevel::INFO, "Opening a {} port on {}",
                 socket_.local_endpoint().port(),
                 socket_.local_endpoint().address().to_string());
  }

  Socket(const string &address, unsigned int port_id,
         unsigned int task_execution_period,
         shared_ptr<ThreadsafeQueue<Message>> incominng_messages,
         shared_ptr<ThreadsafeQueue<Message>> outgoing_messages)
      : Socket(udp::endpoint(ip::make_address(address), port_id),
               task_execution_period, incominng_messages, outgoing_messages) {}

  Socket(bool ip_v6_handler, unsigned int port_id,
         unsigned int task_execution_period,
         shared_ptr<ThreadsafeQueue<Message>> incominng_messages,
         shared_ptr<ThreadsafeQueue<Message>> outgoing_messages)
      : Socket(udp::endpoint(selectProtocol(ip_v6_handler), port_id),
               task_execution_period, incominng_messages, outgoing_messages) {}

  ~Socket() {
    LoggerRepository::getInstance().deregisterLoger(logger_->getName());
  }

  void listen() override {
    auto message = outgoing_messages_->try_pop();
    if (message) {
      send(move(message));
    } else {
      receive();
    }
  }
};

Server::Server()
    : incominng_messages_(make_shared<ThreadsafeQueue<Message>>()),
      outgoing_messages_(make_shared<ThreadsafeQueue<Message>>()),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

Server::Server(const string &ip_address, unsigned int port_id,
               unsigned int task_execution_period)
    : Server() {
  socket_ = make_unique<Socket>(ip_address, port_id, task_execution_period,
                                incominng_messages_, outgoing_messages_);
}

Server::Server(bool ip_v6_handler, unsigned int port_id,
               unsigned int task_execution_period)
    : Server() {
  socket_ = make_unique<Socket>(ip_v6_handler, port_id, task_execution_period,
                                incominng_messages_, outgoing_messages_);
};

Server::~Server() {
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

void Server::run() {
  do {
    try {
      socket_->listen();
    } catch (Network_IO_Exception &ex) {
      logger_->log(SeverityLevel::ERROR, ex.what());
    }
  } while (!stopRequested());
}

unique_ptr<Message> Server::pullRequest() {
  return incominng_messages_->wait_and_pop();
}

void Server::pushResponse(unique_ptr<Message> message) {
  outgoing_messages_->push(move(message));
}

shared_ptr<ThreadsafeQueue<Message>> Server::getIncomingMessagesQueue() {
  return incominng_messages_;
}

shared_ptr<ThreadsafeQueue<Message>> Server::getOutgoingMessagesQueue() {
  return outgoing_messages_;
}
} // namespace CoAP