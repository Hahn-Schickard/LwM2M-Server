#include "PrimitiveConverter.hpp"

#include <stdexcept>

#define MAX_8BIT_VALUE 256
#define MAX_16BIT_VALUE 65536
#define MAX_32BIT_VALUE 4294967296

using namespace std;
namespace utility {

vector<uint8_t> toBytes(const string &value) {
  vector<uint8_t> bytes;
  for (auto it = value.cbegin(); it != value.cend(); ++it)
    bytes.push_back(*it);
  return bytes;
}

vector<uint8_t> toBytes(double value) {
  throw runtime_error("Convertion from double to bytes is not supported!");
}

vector<uint8_t> toBytes(size_t value) {
  vector<uint8_t> result;
  while (value != 0) {
    uint8_t byte = value & 0xFF;
    result.insert(result.begin(), byte);
    value >>= 8;
  }

  return result;
}

vector<uint8_t> toBytes(bool value) { return toBytes((size_t)value); }

vector<uint8_t> toBytes(uint16_t value) { return toBytes((size_t)value); }

vector<uint8_t> toBytes(uint32_t value) { return toBytes((size_t)value); }

vector<uint8_t> toBytes(int64_t value) { return toBytes((size_t)value); }
} // namespace utility
