#ifndef __COAP_MESSAGE_DEFINITION_HPP
#define __COAP_MESSAGE_DEFINITION_HPP

#include "CoAP_Header.hpp"
#include "CoAP_Option.hpp"
#include "Hashers.hpp"
#include "PayloadFormat.hpp"

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
          Header header_data, std::vector<std::shared_ptr<Option>> options,
          std::shared_ptr<PayloadFormat> body);
  Message(std::string receiver_ip, unsigned int receiver_port,
          Header header_data, std::vector<uint8_t> token,
          std::vector<std::shared_ptr<Option>> options,
          std::shared_ptr<PayloadFormat> body);

  friend bool operator==(const Message &lhs, const Message &rhs);
  friend bool operator!=(const Message &lhs, const Message &rhs);

  std::vector<uint8_t> toPacket() const;
  std::string getReceiverIP() const;
  unsigned int getReceiverPort() const;
  const Header getHeader() const;
  std::vector<uint8_t> getToken() const;
  std::vector<std::shared_ptr<Option>> getOptions() const;
  std::shared_ptr<PayloadFormat> getBody() const;
};
} // namespace CoAP

namespace std {
template <> struct hash<CoAP::Message> {
  size_t operator()(const CoAP::Message &value) const {
    size_t ip_hash = hash<string>{}(value.getReceiverIP());
    size_t port_hash = hash<unsigned int>{}(value.getReceiverPort());
    size_t header_hash = hash<CoAP::Header>{}(value.getHeader());
    size_t token_hash = hash<vector<uint8_t>>{}(value.getToken());
    auto options = value.getOptions();
    size_t options_hash = 0;
    for (size_t i = 0; i < options.size(); i++) {
      options_hash |= hash<CoAP::Option>{}(*options[i])
                      << (i * options[i]->size());
    }
    auto payload = value.getBody();
    size_t payload_hash = 0;
    size_t payload_size = 0;
    if (payload) {
      payload_hash = hash<vector<uint8_t>>{}(payload->getBytes());
      payload_size = payload->getBytes().size();
    }

    size_t options_offset = payload_size;
    size_t token_offset = options.size() + options_offset;
    size_t header_offset = value.getToken().size() + token_offset;
    size_t port_offset = 4 + header_offset;
    size_t ip_offset = sizeof(unsigned int) + port_offset;

    return (ip_hash << ip_offset) | (port_hash | port_offset) |
           (header_hash << header_offset) | (token_hash << token_offset) |
           (options_hash << options_offset) | payload_hash;
  }
};
} // namespace std

#endif //__COAP_MESSAGE_DEFINITION_HPP