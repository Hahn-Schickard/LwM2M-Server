#ifndef __COAP_MESSAGE_DEFINITION_HPP
#define __COAP_MESSAGE_DEFINITION_HPP

#include "CoAP_Header.hpp"
#include "CoAP_Option.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace CoAP {

class CoAP_Message {
  std::string receiver_ip_;
  unsigned int receiver_port_;
  CoAP_Header header_;
  std::vector<uint8_t> token_;
  std::vector<std::shared_ptr<CoAP_Option>> options_;
  std::vector<uint8_t> body_;

public:
  CoAP_Message();
  CoAP_Message(std::string receiver_ip, unsigned int receiver_port,
               CoAP_Header header_data, std::vector<uint8_t> body_data);

  std::vector<uint8_t> toPacket();
  std::string getReceiverIP();
  unsigned int getReceiverPort();
  CoAP_Header &getHeader();
  std::vector<uint8_t> getToken();
  std::vector<std::shared_ptr<CoAP_Option>> getOptions();
  std::vector<uint8_t> getBody();
};
} // namespace CoAP
#endif //__COAP_MESSAGE_DEFINITION_HPP