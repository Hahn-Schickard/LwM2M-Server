#ifndef __COAP_OPTION_DEFINITIONS_HPP
#define __COAP_OPTION_DEFINITIONS_HPP

#include <optional>
#include <string>
#include <vector>

namespace CoAP {
typedef enum OptionNumberEnum {
  RESERVED = 0,
  IF_MATCH = 1,
  URI_HOST = 3,
  ETAG = 4,
  IF_NONE_MATCH = 5,
  URI_PORT = 7,
  LOCATION_PATH = 8,
  URI_PATH = 11,
  CONTENT_FORMAT = 12,
  MAX_AGE = 14,
  URI_QUERY = 15,
  ACCEPT = 17,
  LOCATION_QUERY = 20,
  PROXY_URI = 35,
  PROXY_SCHEME = 39,
  SIZE_1 = 60
} OptionNumber;

std::string toString(OptionNumber option);

class CoAP_Option {
  OptionNumber option_number_;
  size_t option_size_;
  bool critical_;
  bool repeatable_;
  bool unsafe_;

protected:
  const size_t max_size_;

public:
  CoAP_Option(OptionNumber option_number, size_t option_size, bool critical,
              bool repeatable, bool unsafe, size_t max_size);
  CoAP_Option();
  virtual ~CoAP_Option() = default;

  virtual std::string getValue() = 0;

  OptionNumber getOptionNumber();
  size_t size();
  bool isCritical();
  bool isRepeatable();
  bool isUnsafe();
};

} // namespace CoAP
#endif //__COAP_OPTION_DEFINITIONS_HPP