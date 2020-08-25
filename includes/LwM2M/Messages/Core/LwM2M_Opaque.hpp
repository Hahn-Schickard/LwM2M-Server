#ifndef __LWM2M_OPAQUE_DATA_FORMAT_TYPE_HPP
#define __LWM2M_OPAQUE_DATA_FORMAT_TYPE_HPP

#include "LwM2M_DataFormat.hpp"

namespace LwM2M {
class Opaque : public DataFormat {
  std::vector<uint8_t> value_;

public:
  Opaque();
  Opaque(std::vector<uint8_t> bytestream);

  std::vector<uint8_t> getValue();
  std::vector<uint8_t> getBytes() override;
  std::string toString() override;
};
} // namespace LwM2M

#endif //__LWM2M_OPAQUE_DATA_FORMAT_TYPE_HPP