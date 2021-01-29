#ifndef __LWM2M_DATA_FORMAT_TYPE_HPP
#define __LWM2M_DATA_FORMAT_TYPE_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <stdexcept>

namespace LwM2M {

enum class ContentFormatType : uint16_t {
  PLAIN_TEXT = 0,
  OPAQUE = 42,
  CBOR = 60,
  SenML_JSON = 110,
  SenML_CBOR = 112,
  TLV = 11542,
  JSON = 11543,
  UNRECOGNIZED
};

class DataFormat {
  ContentFormatType data_format_;

public:
  DataFormat(ContentFormatType format) : data_format_(format) {}
  virtual ~DataFormat() = default;

  ContentFormatType getContentFormatType() { return data_format_; }
  virtual std::vector<uint8_t> getBytes() = 0;
  virtual std::string toString() = 0;
};
} // namespace LwM2M

#endif //__LWM2M_DATA_FORMAT_TYPE_HPP