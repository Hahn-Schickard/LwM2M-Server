#include "LwM2M_PlainText.hpp"

using namespace std;

namespace LwM2M {
PlainText::PlainText() : PlainText(string()) {}

PlainText::PlainText(string value)
    : DataFormat(ContentFormatType::PLAIN_TEXT), value_(value) {}

PlainText::PlainText(vector<uint8_t> bytestream)
    : PlainText(string(bytestream.begin(), bytestream.end())) {}

vector<uint8_t> PlainText::getValue() { return getBytes(); }

vector<uint8_t> PlainText::getBytes() {
  return vector<uint8_t>(value_.begin(), value_.end());
}

string PlainText::toString() { return value_; }

} // namespace LwM2M
