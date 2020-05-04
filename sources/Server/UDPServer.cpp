#include "UDPServer.hpp"

#include <array>
#include <asio.hpp>
#include <functional>
#include <memory>
#include <string>

using namespace std;
using namespace asio;
using asio::ip::udp;
using namespace LwM2M_Server;

udp selectProtocol(bool ip_v6) {
  if (ip_v6) {
    return asio::ip::udp::v6();
  } else {
    return asio::ip::udp::v4();
  }
}

string make_daytime_string() {
  time_t now = time(0);
  return string(ctime(&now));
}

class UDPPort {
  udp::endpoint remote_endpoint_;
  ip::udp::socket socket_;
  std::array<char, 1> buffer_;

  void listen() {
    socket_.async_receive_from(buffer(buffer_), remote_endpoint_,
                               bind(&UDPPort::receive, this,
                                    std::placeholders::_1,
                                    std::placeholders::_2));
  }

  void receive(const error_code &error, std::size_t bytes_transferred) {
    if (!error) {
      auto message = make_shared<string>(make_daytime_string());

      socket_.async_send_to(asio::buffer(*message.get()), remote_endpoint_,
                            bind(&UDPPort::send, this, message,
                                 std::placeholders::_1, std::placeholders::_2));
      listen();
    }
  }

  void send(std::shared_ptr<std::string> message, const error_code &error,
            std::size_t bytes_transferred) {
    // nothing to send!
  }

public:
  UDPPort(bool ip_v6_handler, unsigned int port_id, io_context &io_context)
      : socket_(io_context,
                udp::endpoint(selectProtocol(ip_v6_handler), port_id)) {
    listen();
  }
};

void UDPConnection::run() {
  io_context io_context;
  UDPPort port(ip_v6_handler_, port_id_, io_context);
  do {
    io_context.run_for(asio::chrono::seconds(task_execution_period_));
  } while (!stopRequested());
}