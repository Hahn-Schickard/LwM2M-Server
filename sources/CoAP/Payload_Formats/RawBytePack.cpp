#include "RawBytePack.hpp"

using namespace std;

namespace CoAP {
RawBytePack::RawBytePack(uint16_t content_format, vector<uint8_t> bytes)
    : PayloadFormat(content_format), bytes_(bytes) {}

vector<uint8_t> RawBytePack::getBytes() { return bytes_; }

string RawBytePack::toString() {
  // string of hexes here
}
} // namespace CoAP