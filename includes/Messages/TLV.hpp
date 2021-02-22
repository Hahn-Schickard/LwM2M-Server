#ifndef __LWM2M_TYPE_LENGHT_VALUE_HPP
#define __LWM2M_TYPE_LENGHT_VALUE_HPP

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

namespace LwM2M {
enum class Identifier_Type : uint8_t {
  Object_Instance = 0b00,
  Resource_Instance = 0b01,
  Multiple_Resources = 0b10,
  Resource_Value = 0b11
};

enum class Length_Type : uint8_t {
  No_Length = 0b00,
  Byte_Long = 0b01,
  Short_Long = 0b10,
  Full_Length = 0b11
};

Length_Type getLengthType(std::vector<uint8_t> value);

struct TLV_Header {
  Identifier_Type identifier_;
  bool is_identifier_short_long_;
  Length_Type length_type_;
  uint8_t value_length_;

  TLV_Header();
  TLV_Header(uint8_t byte);
  TLV_Header(Identifier_Type identifier, bool is_identifier_short_long,
             Length_Type length_type, uint8_t value_length);

  uint8_t toByte();
};

using TLV_HeaderPtr = std::shared_ptr<TLV_Header>;

class TLV {
  TLV_HeaderPtr header_;
  uint16_t identifier_;
  uint32_t length_;
  std::vector<uint8_t> value_;

public:
  TLV() = default;
  TLV(std::vector<uint8_t> &bytestream);
  TLV(std::shared_ptr<TLV_Header> header, uint16_t identifier, uint32_t length,
      std::vector<uint8_t> value);

  uint16_t getIdentifier();
  std::vector<uint8_t> getValue();
  std::vector<uint8_t> getBytes();
};

using TLV_ptr = std::shared_ptr<TLV>;
using TLV_ValueMap = std::unordered_map<uint16_t, TLV_ptr>;

class TLV_Pack {
  TLV_ValueMap values_;

  TLV_ptr getTLV(uint16_t identifier);

public:
  TLV_Pack(std::vector<uint8_t> bytestream);
  TLV_Pack(TLV_ValueMap values);

  template <typename T> T getValue(uint16_t identifier) {
    throw std::runtime_error("Can not get value of an unsupported data type!");
  }

  std::vector<uint8_t> getBytes();
};

using TLV_PackPtr = std::shared_ptr<TLV_Pack>;

} // namespace LwM2M

#endif //__LWM2M_TYPE_LENGHT_VALUE_HPP