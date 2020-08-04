#include "PrimitiveConverter.hpp"

using namespace std;
namespace utility {

vector<uint8_t> toBytes(const string &value) {
  vector<uint8_t> bytes;
  for (auto it = value.cbegin(); it != value.cend(); ++it)
    bytes.push_back(*it);
  return bytes;
}

enum class integer_size : uint8_t { SHORT = 2, WORD = 4, LONG = 8 };

vector<uint8_t> unpack_int(uint64_t value, integer_size type) {
  vector<uint8_t> result;
  uint64_t mask;
  uint8_t bytes_to_iterate = static_cast<uint8_t>(type);
  switch (type) {
  case integer_size::SHORT: {
    mask = 0xFF;
    break;
  }
  case integer_size::WORD: {
    mask = 0x0000FF;
    break;
  }
  case integer_size::LONG: {
    mask = 0x00000000000000FF;
    break;
  }
  default: { break; }
  }

  for (int i = 0; i < bytes_to_iterate; i++) {
    result.push_back(value & mask);
    value = value >> 8;
  }

  return result;
}

vector<uint8_t> toBytes(uint16_t value) {
  return unpack_int(value, integer_size::SHORT);
}
vector<uint8_t> toBytes(uint32_t value) {
  return unpack_int(value, integer_size::WORD);
}
vector<uint8_t> toBytes(uint64_t value) {
  return unpack_int(value, integer_size::LONG);
}

} // namespace utility
