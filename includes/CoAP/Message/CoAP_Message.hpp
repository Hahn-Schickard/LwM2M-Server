#ifndef __COAP_MESSAGE_DEFINITION_HPP
#define __COAP_MESSAGE_DEFINITION_HPP

#include "CoAP_Header.hpp"
#include "CoAP_Option.hpp"
#include "PayloadFormat.hpp"
#include "Threadsafe_Queue.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace CoAP {

class Message {
  std::string receiver_ip_;
  unsigned int receiver_port_;
  Header header_;
  std::vector<uint8_t> token_;
  std::vector<std::shared_ptr<Option>> options_;
  std::shared_ptr<PayloadFormat> body_;

  void processToken(std::vector<uint8_t> &udp_datagram);
  void processOptions(std::vector<uint8_t> &udp_datagram);
  void processPayload(std::vector<uint8_t> &udp_datagram);

public:
  Message();
  Message(std::string receiver_ip, unsigned int receiver_port,
          std::vector<uint8_t> udp_datagram);
  Message(std::string receiver_ip, unsigned int receiver_port,
          Header header_data, std::vector<uint8_t> token,
          std::vector<std::shared_ptr<Option>> options,
          std::shared_ptr<PayloadFormat> body);

  std::vector<uint8_t> toPacket();
  std::string getReceiverIP() const;
  unsigned int getReceiverPort() const;
  const Header getHeader() const;
  std::vector<uint8_t> getToken() const;
  std::vector<std::shared_ptr<Option>> getOptions() const;
  std::shared_ptr<PayloadFormat> getBody() const;
};

typedef ThreadsafeQueue<CoAP::Message> MessageBuffer;

} // namespace CoAP
#endif //__COAP_MESSAGE_DEFINITION_HPP