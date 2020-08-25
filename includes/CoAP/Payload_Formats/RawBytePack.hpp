#ifndef __COAP_MESSAGE_PAYLOAD_FORMAT_RAW_BYTE_PACK_HPP
#define __COAP_MESSAGE_PAYLOAD_FORMAT_RAW_BYTE_PACK_HPP

#include "PayloadFormat.hpp"

#include <vector>

namespace CoAP {
class RawBytePack : public PayloadFormat {
  std::vector<uint8_t> bytes_;

public:
  RawBytePack(uint16_t content_format, std::vector<uint8_t> bytes);

  std::vector<uint8_t> getBytes() override;
  std::string toString() override;
};
} // namespace CoAP

#endif //__COAP_MESSAGE_PAYLOAD_FORMAT_RAW_BYTE_PACK_HPP