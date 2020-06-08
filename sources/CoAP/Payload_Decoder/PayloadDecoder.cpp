#include "PayloadDecoder.hpp"
#include "CoRE_Link.hpp"
#include "PlainText.hpp"
#include "StringSpliter.hpp"

using namespace std;

namespace CoAP {

void throwCodingError(string code_format, bool decoding) {
  string error_msg = code_format + (decoding ? " decoding" : " encoding") +
                     " is not supported.";
  throw domain_error(error_msg);
}

string decodeUTF_8(vector<uint8_t> payload) {
  string result;
  for (auto byte : payload) {
    result.push_back(byte);
  }
  return result;
}

shared_ptr<PayloadFormat> decodePlainText(vector<uint8_t> payload) {
  return make_shared<PlainText>(decodeUTF_8(payload));
}
shared_ptr<PayloadFormat> decodeCoRELink(vector<uint8_t> payload) {
  return make_shared<CoRE_Links>(decodeUTF_8(payload));
}
shared_ptr<PayloadFormat> decodeOpaque(vector<uint8_t> payload) {
  throwCodingError("Opaque", true);
}
shared_ptr<PayloadFormat> decodeCBOR(vector<uint8_t> payload) {
  throwCodingError("CBOR", true);
}
shared_ptr<PayloadFormat> decodeSenMLJSON(vector<uint8_t> payload) {
  throwCodingError("SehML JSON", true);
}
shared_ptr<PayloadFormat> decodeSenMLCBOR(vector<uint8_t> payload) {
  throwCodingError("SenML CBOR", true);
}
shared_ptr<PayloadFormat> decodeTLV(vector<uint8_t> payload) {
  throwCodingError("TLV", true);
}
shared_ptr<PayloadFormat> decodeJSON(vector<uint8_t> payload) {
  throwCodingError("JSON", true);
}

shared_ptr<PayloadFormat> decode(shared_ptr<ContentFormat> format,
                                 vector<uint8_t> payload) {
  shared_ptr<PayloadFormat> result;
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

vector<uint8_t> encodePlainText(string payload) {
  throwCodingError("Plain Text", false);
}
vector<uint8_t> encodeCoRELink(string payload) {
  throwCodingError("CoRE Link", false);
}
vector<uint8_t> encodeOpaque(string payload) {
  throwCodingError("Opaque", false);
}
vector<uint8_t> encodeCBOR(string payload) { throwCodingError("CBOR", false); }
vector<uint8_t> encodeSenMLJSON(string payload) {
  throwCodingError("SehML JSON", false);
}
vector<uint8_t> encodeSenMLCBOR(string payload) {
  throwCodingError("SenML CBOR", false);
}
vector<uint8_t> encodeTLV(string payload) { throwCodingError("TLV", false); }
vector<uint8_t> encodeJSON(string payload) { throwCodingError("JSON", false); }

vector<uint8_t> encode(shared_ptr<PayloadFormat> format) {
  vector<uint8_t> result;
  switch (format->getContentFormatType()) {
  case ContentFormatType::PLAIN_TEXT: {
    result = encodePlainText(format->toString());
    break;
  }
  case ContentFormatType::CORE_LINK: {
    result = encodeCoRELink(format->toString());
    break;
  }
  case ContentFormatType::OPAQUE: {
    result = encodeOpaque(format->toString());
    break;
  }
  case ContentFormatType::CBOR: {
    result = encodeCBOR(format->toString());
    break;
  }
  case ContentFormatType::SENML_JSON: {
    result = encodeSenMLJSON(format->toString());
    break;
  }
  case ContentFormatType::SENML_CBOR: {
    result = encodeSenMLCBOR(format->toString());
    break;
  }
  case ContentFormatType::TLV: {
    result = encodeTLV(format->toString());
    break;
  }
  case ContentFormatType::JSON: {
    result = encodeJSON(format->toString());
    break;
  }
  case ContentFormatType::UNRECOGNIZED:
  default: { break; }
    return result;
  }
}

} // namespace CoAP
