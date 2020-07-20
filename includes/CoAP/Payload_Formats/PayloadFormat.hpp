#ifndef __COAP_MESSAGE_PAYLOAD_FORMAT_HPP
#define __COAP_MESSAGE_PAYLOAD_FORMAT_HPP

#include "ContentFormat.hpp"

namespace CoAP {

class PayloadFormat {
  ContentFormatType format_type_;

public:
  PayloadFormat() : PayloadFormat(ContentFormatType::UNRECOGNIZED) {}
  PayloadFormat(ContentFormatType format_type) : format_type_(format_type) {}
  virtual ~PayloadFormat() = default;

  ContentFormatType getContentFormatType() { return format_type_; }
  virtual std::string toString() = 0;
  virtual size_t size() = 0;
};
} // namespace CoAP

#endif //__COAP_MESSAGE_PAYLOAD_FORMAT_HPP