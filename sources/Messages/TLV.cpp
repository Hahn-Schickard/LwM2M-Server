#include "TLV.hpp"

#include <stdexcept>
#include <string>

using namespace std;

namespace LwM2M {

string toString(Identifier_Type type) {
  switch (type) {
  case Identifier_Type::Multiple_Resources:
    return "Multiple Resources";
  case Identifier_Type::Resource_Instance:
    return "Resource Instance";
  case Identifier_Type::Resource_Value:
    return "Resource value";
  case Identifier_Type::Object_Instance:
    [[fallthrough]];
  default:
    return "Object Instance";
  }
}

string toString(Length_Type type) {
  switch (type) {
  case Length_Type::Full_Length:
    return "24 bit long length field";
  case Length_Type::Short_Long:
    return "16 bit long length field";
  case Length_Type::Byte_Long:
    return "8 bit long length field";
  case Length_Type::No_Length:
    [[fallthrough]];
  default:
    return "No length field";
  }
}

static constexpr uint8_t BYTE_LONG = 8;
static constexpr uint8_t SHORT_LONG = 16;
static constexpr uint8_t WORD_LONG = 24;

Length_Type getLengthType(const vector<uint8_t>& value) {
  if (value.size() < BYTE_LONG) {
    return Length_Type::No_Length;
  } else if (value.size() < SHORT_LONG) {
    return Length_Type::Byte_Long;
  } else if (value.size() < WORD_LONG) {
    return Length_Type::Short_Long;
  } else {
    return Length_Type::Full_Length;
  }
}

// NOLINTBEGIN(readability-magic-numbers)
Identifier_Type getIdentifierType(uint8_t byte) {
  return static_cast<Identifier_Type>((byte & 0xC0) >> 6);
}

bool isIdentifierShortLong(uint8_t byte) { return ((byte & 0x20) >> 5) != 0; }

Length_Type getLengthType(uint8_t byte) {
  return static_cast<Length_Type>((byte & 0x18) >> 3);
}

uint8_t getSmallLengthValue(uint8_t byte) { return byte & 0x7; }

TLV_Header::TLV_Header(uint8_t byte)
    : identifier_(getIdentifierType(byte)),
      is_identifier_short_long_(isIdentifierShortLong(byte)),
      length_type_(getLengthType(byte)),
      value_length_(getSmallLengthValue(byte)) {}

TLV_Header::TLV_Header(Identifier_Type identifier,
    bool is_identifier_short_long, Length_Type length_type,
    uint8_t value_length)
    : identifier_(identifier),
      is_identifier_short_long_(is_identifier_short_long),
      length_type_(length_type), value_length_(value_length) {}

// NOLINTBEGIN(readability-implicit-bool-conversion)
uint8_t TLV_Header::toByte() const {
  uint8_t result = 0;
  result |= (static_cast<uint8_t>(identifier_) << 6);
  result |= ((is_identifier_short_long_ & 0x1) << 5);
  result |= (static_cast<uint8_t>(length_type_) << 3);
  result |= (value_length_ & 0x7);
  return result;
}
// NOLINTEND(readability-implicit-bool-conversion)

TLV::TLV(vector<uint8_t>& bytestream) {
  auto it = bytestream.begin();
  header_ = make_shared<TLV_Header>(*it);
  ++it;
  if (header_->is_identifier_short_long_) {
    // MSB first
    identifier_ = (*it << BYTE_LONG) | *(it + 1);
    ++it;
  } else {
    identifier_ = *it;
  }
  ++it;

  switch (header_->length_type_) {
  case Length_Type::No_Length: {
    length_ = header_->value_length_;
    break;
  }
  case Length_Type::Byte_Long: {
    length_ = *it;
    ++it;
    break;
  }
  case Length_Type::Short_Long: {
    length_ = (*it << BYTE_LONG) | *(it + 1);
    it += 2;
    break;
  }
  case Length_Type::Full_Length:
  default: {
    length_ = (*it << BYTE_LONG) | (*(it + 1) << BYTE_LONG) | *(it + 2);
    it += 3;
    break;
  }
  }

  auto value_end = it + length_;
  if (value_end <= bytestream.end()) {
    value_ = vector<uint8_t>(it, value_end);
    bytestream.erase(bytestream.begin(), value_end);
  } else {
    string error_msg = "TLV Value length exceeded bytesream size. TLV Header "
                       "specyfies that the value is " +
        to_string(length_) + " bytes long, but bytesream only contains " +
        to_string(distance(it, bytestream.end())) + " bytes.";
    throw length_error(error_msg);
  }
}

TLV::TLV(const TLV_HeaderPtr& header, uint16_t identifier, uint32_t length,
    const vector<uint8_t>& value)
    : header_(header), identifier_(identifier), length_(length), value_(value) {
}

uint16_t TLV::getIdentifier() const { return identifier_; }

Identifier_Type TLV::getIdentifierType() const { return header_->identifier_; }

vector<uint8_t> TLV::getValue() const { return value_; }

static constexpr uint8_t LSB_MASK = 0xF;
static constexpr uint8_t MSB_MASK = 0xF0;

vector<uint8_t> TLV::getBytes() const {
  vector<uint8_t> result;
  result.push_back(header_->toByte());
  switch (header_->length_type_) {
  case Length_Type::No_Length: {
    break;
  }
  case Length_Type::Byte_Long: {
    uint8_t length_byte = length_;
    result.push_back(length_byte);
    break;
  }
  case Length_Type::Short_Long: {
    uint8_t length_lsb = length_ & LSB_MASK;
    result.push_back(length_lsb);
    uint8_t length_msb = length_ & MSB_MASK >> BYTE_LONG;
    result.push_back(length_msb);
    break;
  }
  case Length_Type::Full_Length:
  default: {
    uint8_t length_lsb = length_ & LSB_MASK;
    result.push_back(length_lsb);
    uint8_t length_middle = length_ & 0xF0 >> BYTE_LONG;
    result.push_back(length_middle);
    uint8_t length_msb = length_ & 0xF00 >> BYTE_LONG;
    result.push_back(length_msb);
    break;
  }
  }
  if (header_->is_identifier_short_long_) {
    uint8_t identifier_lsb = identifier_ & LSB_MASK;
    result.push_back(identifier_lsb);
    uint8_t identifier_msb = identifier_ & MSB_MASK >> BYTE_LONG;
    result.push_back(identifier_msb);
  } else {
    uint8_t identifier_byte = identifier_;
    result.push_back(identifier_byte);
  }
  result.insert(result.end(), value_.begin(), value_.end());
  return result;
}

TLV_Pack::TLV_Pack(vector<uint8_t> bytestream) {
  do {
    TLV_ptr tlv = make_shared<TLV>(bytestream);
    values_.emplace(tlv->getIdentifier(), tlv);
  } while (!bytestream.empty());
}

TLV_Pack::TLV_Pack(const TLV_ValueMap& values) : values_(values) {}

shared_ptr<TLV> TLV_Pack::getTLV(uint16_t identifier) const {
  auto it = values_.find(identifier);
  if (it != values_.end()) {
    return it->second;
  } else {
    throw runtime_error(
        "TLV has no value for identifier:" + to_string(identifier));
  }
}

vector<uint8_t> TLV_Pack::getBytes() const {
  vector<uint8_t> result;
  for (const auto& value : values_) {
    auto byte_pack = value.second->getBytes();
    result.insert(result.end(), byte_pack.begin(), byte_pack.end());
  }
  return result;
}

TLV_ValueMap TLV_Pack::getPack() const { return values_; }

vector<TLV_ptr> TLV_Pack::getPackAsVector() const {
  vector<TLV_ptr> result;
  for (const auto& value : values_) {
    result.push_back(value.second);
  }
  return result;
}
} // namespace LwM2M
// NOLINTEND(readability-magic-numbers)