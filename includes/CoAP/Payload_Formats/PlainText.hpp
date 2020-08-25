#ifndef __COAP_MESSAGE_PAYLOAD_FORMAT_PLAIN_TEXT_HPP
#define __COAP_MESSAGE_PAYLOAD_FORMAT_PLAIN_TEXT_HPP

#include "PayloadFormat.hpp"

#include <string>

namespace CoAP {

class PlainText : public PayloadFormat {
  std::string text_;

public:
  PlainText();
  PlainText(std::vector<uint8_t> bytestream);
  PlainText(std::string utf_8_string);

  std::vector<uint8_t> getBytes() override;
  std::string toString() override;
};

} // namespace CoAP

#endif //__COAP_MESSAGE_PAYLOAD_FORMAT_PLAIN_TEXT_HPP