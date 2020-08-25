#ifndef __LWM2M_TLV_DATA_FORMAT_TYPE_HPP
#define __LWM2M_TLV_DATA_FORMAT_TYPE_HPP

#include "LwM2M_DataFormat.hpp"

#include <memory>

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

struct TLV_Header {
  Identifier_Type identifier_;
  bool is_identifier_short_long_;
  Length_Type length_type_;
  uint8_t value_length_;

  TLV_Header();
  TLV_Header(uint8_t byte);

  uint8_t toByte();
};

class TLV : public DataFormat {
  std::shared_ptr<TLV_Header> header_;
  uint16_t identifier_;
  uint32_t length_;
  std::vector<uint8_t> value_;

public:
  TLV();
  TLV(std::vector<uint8_t> bytestream);

  std::vector<uint8_t> getValue() override;
  std::vector<uint8_t> getBytes() override;
  std::string toString() override;
};
} // namespace LwM2M

#endif //__LWM2M_TLV_DATA_FORMAT_TYPE_HPP