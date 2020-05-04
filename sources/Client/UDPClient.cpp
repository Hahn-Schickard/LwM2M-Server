#include "UDPClient.hpp"

#include <array>
#include <asio.hpp>

using namespace std;
using asio::ip::udp;

udp selectClientProtocol(bool ip_v6) {
  if (ip_v6) {
    return asio::ip::udp::v6();
  } else {
    return asio::ip::udp::v4();
  }
}

string LwM2M_Client::receive(bool ip_v6_handler, const std::string &ip_address,
                             unsigned int port) {
  asio::io_context io_context;
  udp::resolver resolver(io_context);
  udp::endpoint receiver_endpoint =
      udp::endpoint(asio::ip::address::from_string(ip_address), port);

  udp::socket socket(io_context);
  socket.open(selectClientProtocol(ip_v6_handler));

  array<char, 1> send_buf = {{0}};
  socket.send_to(asio::buffer(send_buf), receiver_endpoint);

  array<char, 128> recv_buf;
  udp::endpoint sender_endpoint;
  size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);
  string result = string(recv_buf.data(), len);
  return result;
}