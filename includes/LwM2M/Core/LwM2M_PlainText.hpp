#ifndef __LWM2M_PLAIN_TEXT_DATA_FORMAT_TYPE_HPP
#define __LWM2M_PLAIN_TEXT_DATA_FORMAT_TYPE_HPP

#include "LwM2M_DataFormat.hpp"

namespace LwM2M {
class PlainText : public DataFormat {
  std::string value_;

public:
  PlainText();
  PlainText(std::string value);
  PlainText(std::vector<uint8_t> bytestream);

  std::vector<uint8_t> getBytes() override;
  std::string toString() override;
};
} // namespace LwM2M

#endif //__LWM2M_PLAIN_TEXT_DATA_FORMAT_TYPE_HPP