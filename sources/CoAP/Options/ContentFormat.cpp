#include "ContentFormat.hpp"

#include <stdexcept>

using namespace std;
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
  case 11542: {
    return ContentFormatType::TLV;
  }
  case 11543: {
    return ContentFormatType::JSON;
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
  case ContentFormatType::TLV: {
    return "Type-Length-Value";
  }
  case ContentFormatType::JSON: {
    return "JavaScript Object Notation";
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
    : CoAP_Option(OptionNumber::CONTENT_FORMAT, 2, false, false, false, 2),
      value_(format_type) {}

ContentFormat::ContentFormat(vector<uint8_t> value)
    : CoAP_Option(OptionNumber::CONTENT_FORMAT, value.size(), false, false,
                  false, 2) {
  if (!value.empty()) {
    uint16_t concat_value = 0;
    uint8_t offset = 0;
    for (auto byte : value) {
      concat_value = concat_value | byte << offset;
      offset += 8;
    }
    value_ = toContentFormatType(concat_value);
  }
}

string ContentFormat::getValue() { return toString(value_); }

uint16_t ContentFormat::getValueAsInt() { return value_; }

ContentFormatType ContentFormat::getContentFormatType() { return value_; }

} // namespace CoAP