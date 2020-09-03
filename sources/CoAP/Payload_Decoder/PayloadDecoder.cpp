#include "PayloadDecoder.hpp"
#include "CoRE_Link.hpp"
#include "PlainText.hpp"
#include "RawBytePack.hpp"

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
  case ContentFormatType::UNRECOGNIZED: {
    result = make_shared<RawBytePack>(format->getAsShort(), payload);
  }
  default: { break; }
  }
  return result;
}
} // namespace CoAP
