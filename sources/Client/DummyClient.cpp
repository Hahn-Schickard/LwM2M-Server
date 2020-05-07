#include "DummyClient.hpp"

#include <string>

using namespace std;
using asio::ip::udp;
using namespace CoAP;
namespace LwM2M_Client {
udp selectClientProtocol(bool ip_v6) {
  if (ip_v6) {
    return asio::ip::udp::v6();
  } else {
    return asio::ip::udp::v4();
  }
}

DummyClient::DummyClient(bool ip_v6_handler, const std::string &ip_address,
                         unsigned int port)
    : receiver_endpoint(
          udp::endpoint(asio::ip::address::from_string(ip_address), port)),
      socket(io_context,
             udp::endpoint(selectClientProtocol(ip_v6_handler), 0)) {}

void DummyClient::sendMessage(CoAP_Message message) {
  socket.send_to(asio::buffer(message.toPacket()), receiver_endpoint);
  char reply[4];
  udp::endpoint sender_endpoint;
  size_t reply_length =
      socket.receive_from(asio::buffer(reply, 4), sender_endpoint);
}
} // namespace LwM2M_Client
