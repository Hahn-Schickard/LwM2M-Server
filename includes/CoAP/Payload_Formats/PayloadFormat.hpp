#ifndef __COAP_MESSAGE_PAYLOAD_FORMAT_HPP
#define __COAP_MESSAGE_PAYLOAD_FORMAT_HPP

#include "ContentFormat.hpp"
#include "Hashers.hpp"

namespace CoAP {

class PayloadFormat {
  ContentFormat format_type_;

public:
  PayloadFormat() : PayloadFormat(ContentFormat()) {}
  PayloadFormat(uint16_t format_type)
      : PayloadFormat(ContentFormat(format_type)) {}
  PayloadFormat(ContentFormat format_type) : format_type_(format_type) {}
  virtual ~PayloadFormat() = default;

  friend bool operator==(PayloadFormat &lhs, PayloadFormat &rhs) {
    return (lhs.format_type_ == rhs.format_type_ &&
            lhs.getBytes() == rhs.getBytes());
  }

  ContentFormat getContentFormatType() { return format_type_; }

  virtual std::vector<uint8_t> getBytes() = 0;
  virtual std::string toString() = 0;
};
} // namespace CoAP

namespace std {
template <> struct hash<CoAP::PayloadFormat> {
  size_t operator()(CoAP::PayloadFormat &value) const {
    return hash<vector<uint8_t>>{}(value.getBytes());
  }
};
} // namespace std

#endif //__COAP_MESSAGE_PAYLOAD_FORMAT_HPP