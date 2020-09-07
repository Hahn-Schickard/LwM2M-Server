#ifndef __COAP_OPTION_DEFINITIONS_HPP
#define __COAP_OPTION_DEFINITIONS_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace CoAP {
enum class OptionNumber : uint16_t {
  RESERVED = 0,        // RFC7252
  IF_MATCH = 1,        // RFC7252
  URI_HOST = 3,        // RFC7252
  ETAG = 4,            // RFC7252
  IF_NONE_MATCH = 5,   // RFC7252
  OBSERVE = 6,         // RFC7641
  URI_PORT = 7,        // RFC7252
  LOCATION_PATH = 8,   // RFC7252
  OSCORE = 9,          // RFC8613
  URI_PATH = 11,       // RFC7252
  CONTENT_FORMAT = 12, // RFC7252
  MAX_AGE = 14,        // RFC7252
  URI_QUERY = 15,      // RFC7252
  ACCEPT = 17,         // RFC7252
  LOCATION_QUERY = 20, // RFC7252
  BLOCK_2 = 23,        // RFC7959
  BLOCK_1 = 27,        // RFC7959
  SIZE_2 = 28,         // RFC7959
  PROXY_URI = 35,      // RFC7252
  PROXY_SCHEME = 39,   // RFC7252
  SIZE_1 = 60,         // RFC7252
  NO_RESPONSE = 258    // RFC7967
};

std::string toString(OptionNumber option);

class Option {
  OptionNumber option_number_;
  size_t option_size_;
  bool critical_;
  bool repeatable_;
  bool unsafe_;

protected:
  const size_t max_size_;

public:
  Option(OptionNumber option_number, size_t option_size, bool critical,
         bool repeatable, bool unsafe, size_t max_size);
  Option();
  virtual ~Option() = default;

  friend bool operator==(Option &lhs, Option &rhs);

  virtual std::vector<uint8_t> getValue();
  virtual std::string getAsString();

  OptionNumber getOptionNumber();
  size_t size();
  bool isCritical();
  bool isRepeatable();
  bool isUnsafe();
};
} // namespace CoAP

namespace std {
template <> struct hash<CoAP::Option> {
  size_t operator()(CoAP::Option &value) const {
    size_t opt_number_hash =
        hash<uint16_t>{}((static_cast<uint16_t>(value.getOptionNumber())));
    auto bytes = value.getValue();
    size_t value_hash = 0;
    for (size_t i = 0; i < bytes.size(); i++) {
      auto hashed_byte = hash<uint8_t>{}(bytes[i]);
      value_hash |= hashed_byte << (i * 8);
    }
    return (opt_number_hash << bytes.size() * 8) | value_hash;
  }
};
} // namespace std

#endif //__COAP_OPTION_DEFINITIONS_HPP