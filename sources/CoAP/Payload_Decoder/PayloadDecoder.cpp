#include "PayloadDecoder.hpp"

#include <stdexcept>

using namespace std;

namespace CoAP {

void throwCodingError(string code_format, bool decoding) {
  string error_msg = code_format + (decoding ? " decoding" : " encoding") +
                     " is not supported.";
  throw domain_error(error_msg);
}

vector<string> decodePlainText(vector<uint8_t> payload) {
  throwCodingError("Plain Text", true);
}
vector<string> decodeCoRELink(vector<uint8_t> payload) {
  throwCodingError("CoRE Link", true);
}
vector<string> decodeOpaque(vector<uint8_t> payload) {
  throwCodingError("Opaque", true);
}
vector<string> decodeCBOR(vector<uint8_t> payload) {
  throwCodingError("CBOR", true);
}
vector<string> decodeSenMLJSON(vector<uint8_t> payload) {
  throwCodingError("SehML JSON", true);
}
vector<string> decodeSenMLCBOR(vector<uint8_t> payload) {
  throwCodingError("SenML CBOR", true);
}
vector<string> decodeTLV(vector<uint8_t> payload) {
  throwCodingError("TLV", true);
}
vector<string> decodeJSON(vector<uint8_t> payload) {
  throwCodingError("JSON", true);
}

vector<string> decode(shared_ptr<ContentFormat> format,
                      vector<uint8_t> payload) {
  vector<string> result;
  switch (format->getContentFormatType()) {
  case ContentFormatType::PLAIN_TEXT: {
    result = decodePlainText(payload);
    break;
  }
  case ContentFormatType::CORE_LINK: {
    result = decodeCoRELink(payload);
    break;
  }
  case ContentFormatType::OPAQUE: {
    result = decodeOpaque(payload);
    break;
  }
  case ContentFormatType::CBOR: {
    result = decodeCBOR(payload);
    break;
  }
  case ContentFormatType::SENML_JSON: {
    result = decodeSenMLJSON(payload);
    break;
  }
  case ContentFormatType::SENML_CBOR: {
    result = decodeSenMLCBOR(payload);
    break;
  }
  case ContentFormatType::TLV: {
    result = decodeTLV(payload);
    break;
  }
  case ContentFormatType::JSON: {
    result = decodeJSON(payload);
    break;
  }
  case ContentFormatType::UNRECOGNIZED:
  default: { break; }
    return result;
  }
}

vector<uint8_t> encodePlainText(vector<string> payload) {
  throwCodingError("Plain Text", false);
}
vector<uint8_t> encodeCoRELink(vector<string> payload) {
  throwCodingError("CoRE Link", false);
}
vector<uint8_t> encodeOpaque(vector<string> payload) {
  throwCodingError("Opaque", false);
}
vector<uint8_t> encodeCBOR(vector<string> payload) {
  throwCodingError("CBOR", false);
}
vector<uint8_t> encodeSenMLJSON(vector<string> payload) {
  throwCodingError("SehML JSON", false);
}
vector<uint8_t> encodeSenMLCBOR(vector<string> payload) {
  throwCodingError("SenML CBOR", false);
}
vector<uint8_t> encodeTLV(vector<string> payload) {
  throwCodingError("TLV", false);
}
vector<uint8_t> encodeJSON(vector<string> payload) {
  throwCodingError("JSON", false);
}

vector<uint8_t> encode(shared_ptr<ContentFormat> format,
                       vector<string> payload) {
  vector<uint8_t> result;
  switch (format->getContentFormatType()) {
  case ContentFormatType::PLAIN_TEXT: {
    result = encodePlainText(payload);
    break;
  }
  case ContentFormatType::CORE_LINK: {
    result = encodeCoRELink(payload);
    break;
  }
  case ContentFormatType::OPAQUE: {
    result = encodeOpaque(payload);
    break;
  }
  case ContentFormatType::CBOR: {
    result = encodeCBOR(payload);
    break;
  }
  case ContentFormatType::SENML_JSON: {
    result = encodeSenMLJSON(payload);
    break;
  }
  case ContentFormatType::SENML_CBOR: {
    result = encodeSenMLCBOR(payload);
    break;
  }
  case ContentFormatType::TLV: {
    result = encodeTLV(payload);
    break;
  }
  case ContentFormatType::JSON: {
    result = encodeJSON(payload);
    break;
  }
  case ContentFormatType::UNRECOGNIZED:
  default: { break; }
    return result;
  }
}

} // namespace CoAP
