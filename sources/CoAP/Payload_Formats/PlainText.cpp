#include "PlainText.hpp"

using namespace std;

namespace CoAP {
PlainText::PlainText() : PlainText(string()) {}

PlainText::PlainText(string utf_8_string)
    : PayloadFormat(ContentFormatType::PLAIN_TEXT), text_(utf_8_string) {}

string PlainText::toString() { return text_; }
} // namespace CoAP