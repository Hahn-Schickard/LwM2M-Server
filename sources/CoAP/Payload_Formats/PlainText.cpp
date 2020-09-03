#include "PlainText.hpp"

using namespace std;

namespace CoAP {
PlainText::PlainText() : PlainText(string()) {}

PlainText::PlainText(vector<uint8_t> bytestream)
    : PayloadFormat(ContentFormatType::PLAIN_TEXT),
      text_(bytestream.begin(), bytestream.end()) {}

PlainText::PlainText(string utf_8_string)
    : PayloadFormat(ContentFormatType::PLAIN_TEXT), text_(utf_8_string) {}

vector<uint8_t> PlainText::getBytes() {
  return vector<uint8_t>(text_.begin(), text_.end());
}

string PlainText::toString() { return text_; }
} // namespace CoAP