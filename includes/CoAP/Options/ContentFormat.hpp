#ifndef __COAP_OPTION_CONTENT_FORMAT_DEFINITION_HPP
#define __COAP_OPTION_CONTENT_FORMAT_DEFINITION_HPP

#include "CoAP_Option.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace CoAP {
enum class ContentFormatType : uint16_t {
  PLAIN_TEXT = 0,
  CORE_LINK = 40,
  OPAQUE = 42,
  CBOR = 60,
  SENML_JSON = 110,
  SENML_CBOR = 112,
  UNRECOGNIZED
};

ContentFormatType toContentFormatType(uint16_t value);
std::string toString(ContentFormatType type);

class ContentFormat : public Option {
  uint16_t value_;

public:
  ContentFormat();
  ContentFormat(ContentFormatType format_type);
  ContentFormat(std::vector<uint8_t> value);
  ContentFormat(uint16_t value);

  std::vector<uint8_t> getValue() override;
  std::string getAsString() override;
  uint16_t getAsShort();
  ContentFormatType getContentFormatType();
};
} // namespace CoAP

#endif //__COAP_OPTION_CONTENT_FORMAT_DEFINITION_HPP