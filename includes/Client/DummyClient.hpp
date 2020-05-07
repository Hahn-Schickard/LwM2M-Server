#ifndef _LwM2M_DUMMY_CLIENT_HPP
#define _LwM2M_DUMMY_CLIENT_HPP

#include "CoAP_Message.hpp"

#include <asio.hpp>
#include <string>

namespace LwM2M_Client {
class DummyClient {
  asio::io_context io_context;
  asio::ip::udp::endpoint receiver_endpoint;
  asio::ip::udp::socket socket;

public:
  DummyClient(bool ip_v6_handler, const std::string &ip_address,
              unsigned int port);

  void sendMessage(CoAP::CoAP_Message message);
};
} // namespace LwM2M_Client
#endif //_LwM2M_DUMMY_CLIENT_HPP
