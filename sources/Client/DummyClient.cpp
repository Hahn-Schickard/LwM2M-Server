#include "DummyClient.hpp"

#include <iostream>
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

void printCoAPHeader(CoAP_Header header) {
  cout << toString(header.getMesageType()) << " Message type with "
       << toString(header.getCodeType()) << endl;
  cout << "Message id is: " << header.getMessageID() << endl;
}

void printOutgoing(CoAP_Message message) {
  cout << "Sending a message to: " << message.getReceiverIP() << ":"
       << message.getReceiverPort() << endl;
  printCoAPHeader(message.getHeader());
  for (const auto &byte : message.getBody()) {
    cout << byte << " ";
  }
  cout << endl;
}

void printIncoming(CoAP_Message message) {
  cout << "Received a message from: " << message.getReceiverIP() << ":"
       << message.getReceiverPort() << endl;
  printCoAPHeader(message.getHeader());
  for (const auto &byte : message.getBody()) {
    cout << byte << " ";
  }
  cout << endl;
}

DummyClient::DummyClient(bool ip_v6_handler, const std::string &ip_address,
                         unsigned int port)
    : receiver_endpoint(
          udp::endpoint(asio::ip::address::from_string(ip_address), port)),
      socket(io_context,
             udp::endpoint(selectClientProtocol(ip_v6_handler), 0)) {}

void DummyClient::sendMessage(CoAP_Message message) {
  printOutgoing(message);
  socket.send_to(asio::buffer(message.toPacket()), receiver_endpoint);
}
} // namespace LwM2M_Client
