#include "CoAP_Server.hpp"
#include "LoggerRepository.hpp"

#include <asio.hpp>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <vector>

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

shared_ptr<CoAP_Message> makeAcknowledgementMessage(udp::endpoint receiver,
                                                    uint16_t message_id) {
  return make_shared<CoAP_Message>(
      receiver.address().to_string(), receiver.port(),
      CoAP_Header(MessageType::ACKNOWLEDGMENT, 0, CodeType::OK, message_id),
      vector<char>());
}

class CoAP_Port {
  ip::udp::socket socket_;
  unsigned int task_execution_period_;
  shared_ptr<ThreadsafeQueue<CoAP_Message>> incominng_messages_;
  shared_ptr<ThreadsafeQueue<CoAP_Message>> outgoing_messages_;
  shared_ptr<Logger> logger_;

  void receive() {
    vector<char> header(HEADER_SIZE);
    udp::endpoint remote_endpoint;

    future<size_t> header_future = socket_.async_receive_from(
        asio::buffer(header, HEADER_SIZE), remote_endpoint, asio::use_future);

    if (header_future.wait_for(asio::chrono::seconds(task_execution_period_)) ==
        future_status::ready) {
      CoAP_Header coap_header(move(header));
      vector<char> body(coap_header.getTokenLenght());
      if (coap_header.getTokenLenght() > 0) {
        future<size_t> body_future = socket_.async_receive_from(
            asio::buffer(body, coap_header.getTokenLenght()), remote_endpoint,
            asio::use_future);
        body_future.wait_for(asio::chrono::seconds(task_execution_period_));
      }
      incominng_messages_->push(
          CoAP_Message(remote_endpoint.address().to_string(),
                       remote_endpoint.port(), move(coap_header), body));
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
  CoAP_Port(bool ip_v6_handler, unsigned int port_id, io_context &io_context,
            unsigned int task_execution_period,
            shared_ptr<ThreadsafeQueue<CoAP_Message>> incominng_messages,
            shared_ptr<ThreadsafeQueue<CoAP_Message>> outgoing_messages)
      : socket_(io_context,
                udp::endpoint(selectProtocol(ip_v6_handler), port_id)),
        task_execution_period_(task_execution_period),
        incominng_messages_(incominng_messages),
        outgoing_messages_(outgoing_messages),
        logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

  ~CoAP_Port() {
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
};

CoAP_Server::CoAP_Server(bool ip_v6_handler, unsigned int port_id,
                         unsigned int task_execution_period)
    : ip_v6_handler_(ip_v6_handler), port_id_(port_id),
      task_execution_period_(task_execution_period),
      incominng_messages_(make_shared<ThreadsafeQueue<CoAP_Message>>()),
      outgoing_messages_(make_shared<ThreadsafeQueue<CoAP_Message>>()),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)){};

CoAP_Server::~CoAP_Server() {
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

void CoAP_Server::run() {
  io_context io_context;
  CoAP_Port port(ip_v6_handler_, port_id_, io_context, task_execution_period_,
                 incominng_messages_, outgoing_messages_);
  do {
    try {
      port.listen();
    } catch (Network_IO_Exception &ex) {
      logger_->log(SeverityLevel::ERROR, ex.what());
      port.listen();
    }
  } while (!stopRequested());
}

shared_ptr<CoAP_Message> CoAP_Server::pullRequest() {
  return incominng_messages_->wait_and_pop();
}

void CoAP_Server::pushResponse(CoAP_Message &message) {
  outgoing_messages_->push(move(message));
}

shared_ptr<ThreadsafeQueue<CoAP_Message>>
CoAP_Server::getIncomingMessagesQueue() {
  return incominng_messages_;
}

shared_ptr<ThreadsafeQueue<CoAP_Message>>
CoAP_Server::getOutgoingMessagesQueue() {
  return outgoing_messages_;
}
} // namespace LwM2M_Server