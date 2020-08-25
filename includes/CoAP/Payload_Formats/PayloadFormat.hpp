#ifndef __COAP_MESSAGE_PAYLOAD_FORMAT_HPP
#define __COAP_MESSAGE_PAYLOAD_FORMAT_HPP

#include "ContentFormat.hpp"

namespace CoAP {

class PayloadFormat {
  ContentFormat format_type_;

public:
  PayloadFormat() : PayloadFormat(ContentFormat()) {}
  PayloadFormat(uint16_t format_type)
      : PayloadFormat(ContentFormat(format_type)) {}
  PayloadFormat(ContentFormat format_type) : format_type_(format_type) {}
  virtual ~PayloadFormat() = default;

  ContentFormat getContentFormatType() { return format_type_; }

  virtual std::vector<uint8_t> getBytes() = 0;
  virtual std::string toString() = 0;
};
} // namespace CoAP

#endif //__COAP_MESSAGE_PAYLOAD_FORMAT_HPP