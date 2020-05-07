#include "CoAP_Server.hpp"
#include "LoggerRepository.hpp"

#include <array>
#include <asio.hpp>
#include <functional>
#include <memory>
#include <string>

#define HEADER_SIZE 4

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
  ip::udp::socket socket_;
  shared_ptr<ThreadsafeQueue<CoAP_Message>> incominng_messages_;
  shared_ptr<ThreadsafeQueue<CoAP_Message>> outgoing_messages_;

  void receive() {
    std::vector<char> header(HEADER_SIZE);
    udp::endpoint remote_endpoint;
    asio::error_code return_code;

    socket_.receive_from(asio::buffer(header, HEADER_SIZE), remote_endpoint, 0,
                         return_code);
    handleReturnCode("Could not receive packet header due to error:",
                     return_code);

    if (header[0] != 0) {
      CoAP_Header coap_header(move(header));
      std::vector<char> body(coap_header.getTokenLenght());
      if (coap_header.getTokenLenght() > 0) {
        socket_.receive_from(asio::buffer(body, coap_header.getTokenLenght()),
                             remote_endpoint, 0, return_code);
        handleReturnCode("Could not receive packet body due to error:",
                         return_code);
        // send ack if confirmable here
      }
      incominng_messages_->push(
          CoAP_Message(remote_endpoint.address().to_string(),
                       remote_endpoint.port(), move(coap_header), body));
    }
    run();
  }

  void send() {
    auto message = outgoing_messages_->wait_and_pop();
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
    run();
  }

public:
  CoAP_Port(bool ip_v6_handler, unsigned int port_id, io_context &io_context,
            shared_ptr<ThreadsafeQueue<CoAP_Message>> incominng_messages,
            shared_ptr<ThreadsafeQueue<CoAP_Message>> outgoing_messages)
      : socket_(io_context,
                udp::endpoint(selectProtocol(ip_v6_handler), port_id)),
        incominng_messages_(incominng_messages),
        outgoing_messages_(outgoing_messages) {}

  void run() {
    if (!outgoing_messages_->empty()) {
      send();
    } else {
      receive();
    }
  }
};

CoAP_Server::CoAP_Server(bool ip_v6_handler, unsigned int port_id,
                         unsigned int task_execution_period)
    : ip_v6_handler_(ip_v6_handler), port_id_(port_id),
      task_execution_period_(task_execution_period),
      exitFuture_(exitSignal_.get_future()),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)){};

CoAP_Server::~CoAP_Server() {
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

void CoAP_Server::run() {
  io_context io_context;
  CoAP_Port port(ip_v6_handler_, port_id_, io_context, incominng_messages_,
                 outgoing_messages_);
  do {
    try {
      port.run();
    } catch (Network_IO_Exception &ex) {
      logger_->log(SeverityLevel::ERROR, ex.what());
      port.run();
    }
  } while (!stopRequested());
}

shared_ptr<CoAP_Message> CoAP_Server::pullRequest() {
  return incominng_messages_->wait_and_pop();
}

void CoAP_Server::pushResponse(CoAP_Message &message) {
  outgoing_messages_->push(move(message));
}
} // namespace LwM2M_Server