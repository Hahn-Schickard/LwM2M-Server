#ifndef __LWM2M_TLV_DATA_FORMAT_TYPE_HPP
#define __LWM2M_TLV_DATA_FORMAT_TYPE_HPP

#include "LwM2M_DataFormat.hpp"

#include <exception>
#include <memory>
#include <unordered_map>

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

class TLV {
  std::shared_ptr<TLV_Header> header_;
  uint16_t identifier_;
  uint32_t length_;
  std::vector<uint8_t> value_;

public:
  TLV();
  TLV(std::vector<uint8_t> &bytestream);

  uint16_t getIdentifier();
  std::vector<uint8_t> getValue();
  std::vector<uint8_t> getBytes();
  std::string toString();
};

class TLV_Pack : public DataFormat {
  using TLV_ptr = std::shared_ptr<TLV>;
  std::unordered_map<uint16_t, TLV_ptr> values_;

  TLV_ptr getTLV(uint16_t identifier);

public:
  TLV_Pack(std::vector<uint8_t> bytestream);

  template <typename T> T getValue(uint16_t identifier) {
    throw std::runtime_error("Can not get value of an unsupported data type!");
  }

  std::vector<uint8_t> getBytes() override;
  std::string toString() override;
};
} // namespace LwM2M

#endif //__LWM2M_TLV_DATA_FORMAT_TYPE_HPP