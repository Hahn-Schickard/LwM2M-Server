#include "ContentFormat.hpp"
#include "PrimitiveConverter.hpp"

#include <stdexcept>

using namespace std;
using namespace utility;
namespace CoAP {
ContentFormatType toContentFormatType(uint16_t value) {
  switch (value) {
  case 0: {
    return ContentFormatType::PLAIN_TEXT;
  }
  case 40: {
    return ContentFormatType::CORE_LINK;
  }
  case 42: {
    return ContentFormatType::OPAQUE;
  }
  case 60: {
    return ContentFormatType::CBOR;
  }
  case 110: {
    return ContentFormatType::SENML_JSON;
  }
  case 112: {
    return ContentFormatType::SENML_CBOR;
  }
  default: {
    string error_msg = "Unsupported content format type: " + to_string(value);
    throw domain_error(move(error_msg));
  }
  }
}

string toString(ContentFormatType type) {
  switch (type) {
  case ContentFormatType::PLAIN_TEXT: {
    return "Plain Text";
  }
  case ContentFormatType::CORE_LINK: {
    return "Constrained RESTful Environments (CoRE) Link Format";
  }
  case ContentFormatType::OPAQUE: {
    return "Opaque";
  }
  case ContentFormatType::CBOR: {
    return "Concise Binary Object Representation";
  }
  case ContentFormatType::SENML_JSON: {
    return "Sensor Measurement Lists over JavaScript Object Notation";
  }
  case ContentFormatType::SENML_CBOR: {
    return "Sensor Measurement Lists over Concise Binary Object Representation";
  }
  default: { return "Unsupported"; }
  }
}

ContentFormat::ContentFormat()
    : ContentFormat(ContentFormatType::UNRECOGNIZED) {}

ContentFormat::ContentFormat(ContentFormatType format_type)
    : Option(OptionNumber::CONTENT_FORMAT, 2, false, false, false, 2),
      value_(static_cast<uint16_t>(format_type)) {}

ContentFormat::ContentFormat(vector<uint8_t> value)
    : Option(OptionNumber::CONTENT_FORMAT, value.size(), false, false, false,
             2) {
  if (!value.empty()) {
    uint16_t concat_value = 0;
    uint8_t offset = 0;
    for (auto byte : value) {
      concat_value = concat_value | byte << offset;
      offset += 8;
    }
    value_ = concat_value;
  }
}

ContentFormat::ContentFormat(uint16_t value)
    : Option(OptionNumber::CONTENT_FORMAT, 2, false, false, false, 2),
      value_(static_cast<uint16_t>(value)) {}

vector<uint8_t> ContentFormat::getValue() { return toBytes(getAsShort()); }

string ContentFormat::getAsString() { return toString(getContentFormatType()); }

uint16_t ContentFormat::getAsShort() { return value_; }

ContentFormatType ContentFormat::getContentFormatType() {
  return toContentFormatType(value_);
}

} // namespace CoAP