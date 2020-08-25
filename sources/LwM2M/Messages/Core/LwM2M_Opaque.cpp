#include "LwM2M_Opaque.hpp"

using namespace std;

namespace LwM2M {
Opaque::Opaque() : Opaque(vector<uint8_t>()) {}

Opaque::Opaque(vector<uint8_t> bytestream)
    : DataFormat(ContentFormatType::OPAQUE), value_(bytestream) {}

vector<uint8_t> Opaque::getValue() { return value_; }

vector<uint8_t> Opaque::getBytes() { return getValue(); }

//@TODO: implement Base64 to string convertion
string Opaque::toString() { return string(value_.begin(), value_.end()); }
} // namespace LwM2M