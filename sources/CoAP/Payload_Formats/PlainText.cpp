#include "PlainText.hpp"

using namespace std;

namespace CoAP {
PlainText::PlainText() : PlainText(string()) {}

PlainText::PlainText(vector<uint8_t> bytestream)
    : PayloadFormat(ContentFormatType::PLAIN_TEXT),
      text_(bytestream.begin(), bytestream.end()) {}

PlainText::PlainText(string utf_8_string)
    : PayloadFormat(ContentFormatType::PLAIN_TEXT), text_(utf_8_string) {}

string PlainText::toString() { return text_; }

size_t PlainText::size() { return text_.length(); }
} // namespace CoAP