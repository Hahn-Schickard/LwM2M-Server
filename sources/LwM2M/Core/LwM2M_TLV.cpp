#include "LwM2M_TLV.hpp"
#include "LwM2M_CoreLink.hpp"
#include "LwM2M_ObjectLink.hpp"

#include <algorithm>

using namespace std;

namespace LwM2M {

Identifier_Type getIdentifierType(uint8_t byte) {
  return static_cast<Identifier_Type>(byte & 0xC0 >> 6);
}

bool isIdentifierSHortLong(uint8_t byte) { return byte & 0x20 >> 5; }

Length_Type getLengthType(uint8_t byte) {
  return static_cast<Length_Type>(byte & 0x18 >> 3);
}

uint8_t getSmallLengthValue(uint8_t byte) { return byte & 0x7; }

TLV_Header::TLV_Header(uint8_t byte)
    : identifier_(getIdentifierType(byte)),
      is_identifier_short_long_(isIdentifierSHortLong(byte)),
      length_type_(getLengthType(byte)),
      value_length_(getSmallLengthValue(byte)) {}

uint8_t TLV_Header::toByte() {
  uint8_t result = 0;
  result |= (static_cast<uint8_t>(identifier_) << 6);
  result |= ((is_identifier_short_long_ & 0x1) << 5);
  result |= (static_cast<uint8_t>(length_type_) << 3);
  result |= (value_length_ & 0x7);
  return result;
}

TLV::TLV() : TLV(vector<uint8_t>()) {}

TLV::TLV(vector<uint8_t> bytestream) {
  auto it = bytestream.begin();
  header_ = make_shared<TLV_Header>(*it);
  it++;
  if (header_->is_identifier_short_long_) {
    // MSB first
    identifier_ = (*it << 8) | *(it + 1);
    it++;
  } else {
    identifier_ = *it;
  }
  it++;
  switch (header_->length_type_) {
  case Length_Type::No_Length: {
    length_ = header_->value_length_;
    break;
  }
  case Length_Type::Byte_Long: {
    length_ = *it;
    break;
  }
  case Length_Type::Short_Long: {
    length_ = (*it << 8) | *(it + 1);
    it++;
    break;
  }
  case Length_Type::Full_Length:
  default: {
    length_ = (*it << 16) | (*(it + 1) << 8) | *(it + 2);
    it = it + 2;
    break;
  }
  }
  it++;
  value_ = vector<uint8_t>(it, it + length_);
  bytestream.erase(bytestream.begin(), it + length_);
}

uint16_t TLV::getIdentifier() { return identifier_; }

vector<uint8_t> TLV::getValue() { return value_; }

vector<uint8_t> TLV::getBytes() {
  vector<uint8_t> result = value_;
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
    uint8_t length_lsb = length_ & 0xF;
    result.push_back(length_lsb);
    uint8_t length_msb = length_ & 0xF0 >> 8;
    result.push_back(length_msb);
    break;
  }
  case Length_Type::Full_Length:
  default: {
    uint8_t length_lsb = length_ & 0xF;
    result.push_back(length_lsb);
    uint8_t length_middle = length_ & 0xF0 >> 8;
    result.push_back(length_middle);
    uint8_t length_msb = length_ & 0xF00 >> 16;
    result.push_back(length_msb);
    break;
  }
  }
  if (header_->is_identifier_short_long_) {
    uint8_t identifier_lsb = identifier_ & 0xF;
    result.push_back(identifier_lsb);
    uint8_t identifier_msb = identifier_ & 0xF0 >> 8;
    result.push_back(identifier_msb);
  } else {
    uint8_t identifier_byte = identifier_;
    result.push_back(identifier_byte);
  }
  result.push_back(header_->toByte());
  return result;
}

string TLV::toString() {
  //@TODO: implement hexification
  return string();
}

TLV_Pack::TLV_Pack(vector<uint8_t> bytestream)
    : DataFormat(ContentFormatType::TLV) {
  do {
    TLV_ptr tlv = make_shared<TLV>(bytestream);
    values_.emplace(tlv->getIdentifier(), tlv);
  } while (!bytestream.empty());
}

shared_ptr<TLV> TLV_Pack::getTLV(uint16_t identifier) {
  auto it = values_.find(identifier);
  if (it != values_.end()) {
    return it->second;
  } else {
    throw runtime_error("TLV has no value for identifier:" +
                        to_string(identifier));
  }
}

template <> void TLV_Pack::getValue<void>(uint16_t identifier) {}

template <>
vector<uint8_t> TLV_Pack::getValue<vector<uint8_t>>(uint16_t identifier) {
  return getTLV(identifier)->getValue();
}

template <> string TLV_Pack::getValue<string>(uint16_t identifier) {
  vector<uint8_t> tlv_value = getTLV(identifier)->getValue();
  return string(tlv_value.begin(), tlv_value.end());
}

template <> bool TLV_Pack::getValue<bool>(uint16_t identifier) {
  vector<uint8_t> tlv_value = getTLV(identifier)->getValue();
  if (!tlv_value.empty() && tlv_value.size() < 2) {
    return (bool)tlv_value[0];
  } else {
    throw logic_error("Bool type can not be empty or larger than 8bits!");
  }
}

template <> int64_t TLV_Pack::getValue<int64_t>(uint16_t identifier) {
  vector<uint8_t> tlv_value = getTLV(identifier)->getValue();
  if (!tlv_value.empty() && tlv_value.size() < 8) {
    int64_t concat_value = 0;
    uint8_t offset = 0;
    for (auto byte : tlv_value) {
      concat_value = concat_value | byte << offset;
      offset += 8;
    }
    return concat_value;
  } else {
    throw logic_error("Intiger type can not be empty or larger than 64bits!");
  }
}

template <> uint64_t TLV_Pack::getValue<uint64_t>(uint16_t identifier) {
  vector<uint8_t> tlv_value = getTLV(identifier)->getValue();
  if (!tlv_value.empty() && tlv_value.size() < 8) {
    uint64_t concat_value = 0;
    uint8_t offset = 0;
    for (auto byte : tlv_value) {
      concat_value = concat_value | byte << offset;
      offset += 8;
    }
    return concat_value;
  } else {
    throw logic_error(
        "Unsigned Intiger type can not be empty or larger than 64bits!");
  }
}

template <> double TLV_Pack::getValue<double>(uint16_t identifier) {
  vector<uint8_t> tlv_value = getTLV(identifier)->getValue();
  double result;
  if (tlv_value.size() <= 8) {
    copy(tlv_value.begin(), tlv_value.end(),
         reinterpret_cast<uint8_t *>(&result));
  } else {
    throw logic_error("Floating point type can not be smaller than 32bits!");
  }
  return result;
}

template <> ObjectLink TLV_Pack::getValue<ObjectLink>(uint16_t identifier) {
  vector<uint8_t> tlv_value = getTLV(identifier)->getValue();
  //@TODO: check how ObjectLink works
  return ObjectLink();
}

template <> CoRE_Link TLV_Pack::getValue<CoRE_Link>(uint16_t identifier) {
  string value = getValue<string>(identifier);
  auto link_value = CoRE_Link(value);
  return link_value;
}

vector<uint8_t> TLV_Pack::getBytes() {
  vector<uint8_t> result;
  for (auto value : values_) {
    auto byte_pack = value.second->getBytes();
    result.insert(result.end(), byte_pack.begin(), byte_pack.end());
  }
  return result;
}

string TLV_Pack::toString() {
  string result;
  for (auto value : values_) {
    result += value.second->toString();
  }
  return result;
}
} // namespace LwM2M