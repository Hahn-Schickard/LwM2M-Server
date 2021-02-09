#include "DataFormat.hpp"

using namespace std;

namespace LwM2M {

string toString(MediaType type) {
  switch (type) {
  case MediaType::PLAIN_TEXT: {
    return "Plain Text";
  }
  case MediaType::CORE_LINK: {
    return "CoRE Link";
  }
  case MediaType::OPAQUE: {
    return "Opaque";
  }
  case MediaType::CBOR: {
    return "CBOR";
  }
  case MediaType::SENML_JSON: {
    return "SenML-JSON";
  }
  case MediaType::SENML_CBOR: {
    return "SenML-CBOR";
  }
  case MediaType::TLV: {
    return "LwM2M TLV";
  }
  case MediaType::JSON: {
    return "LwM2M JSON";
  }
  case MediaType::NOT_SPECIFIED:
  default: { return "Not specified"; }
  }
}

string toString(DataType type) {
  switch (type) {
  case DataType::STRING: {
    return "String";
  }
  case DataType::SIGNED_INTEGER: {
    return "Signed Integer";
  }
  case DataType::UNSIGNED_INTEGER: {
    return "Unsigned Integer";
  }
  case DataType::FLOAT: {
    return "Float";
  }
  case DataType::BOOLEAN: {
    return "Boolean";
  }
  case DataType::OPAQUE: {
    return "Opaque";
  }
  case DataType::TIME: {
    return "POSIX Tine";
  }
  case DataType::OBJECT_LINK: {
    return "Object Link";
  }
  case DataType::CORE_LINK: {
    return "Core Link";
  }
  case DataType::NONE:
  default: { return "None"; }
  }
}

UnsuportedDataType::UnsuportedDataType()
    : logic_error("Tried to access unhandeled data type!") {}

WrongDataType::WrongDataType(DataType expected, DataType actual)
    : logic_error(
          "Wrong actual data type!\nExpected data type: " + toString(expected) +
          "\nActual data type: " + toString(actual)) {}

DataFormat::DataFormat(DataVariant data, DataType type, MediaType format)
    : data_(data), data_type_(type), media_type_(format) {}

template <> void DataFormat::get<void>() {
  if (data_type_ != DataType::NONE) {
    throw WrongDataType(DataType::NONE, data_type_);
  }
}

template <> bool DataFormat::get<bool>() {
  if (data_type_ == DataType::BOOLEAN) {
    return std::get<bool>(data_);
  } else {
    throw WrongDataType(DataType::BOOLEAN, data_type_);
  }
}

template <> int64_t DataFormat::get<int64_t>() {
  if (data_type_ == DataType::SIGNED_INTEGER) {
    return std::get<int64_t>(data_);
  } else {
    throw WrongDataType(DataType::SIGNED_INTEGER, data_type_);
  }
}

template <> uint64_t DataFormat::get<uint64_t>() {
  if (data_type_ == DataType::UNSIGNED_INTEGER ||
      data_type_ == DataType::TIME) {
    return std::get<uint64_t>(data_);
  } else {
    throw WrongDataType(DataType::UNSIGNED_INTEGER, data_type_);
  }
}

template <> double DataFormat::get<double>() {
  if (data_type_ == DataType::FLOAT) {
    return std::get<double>(data_);
  } else {
    throw WrongDataType(DataType::FLOAT, data_type_);
  }
}

template <> string DataFormat::get<string>() {
  if (data_type_ == DataType::STRING) {
    return std::get<string>(data_);
  } else {
    throw WrongDataType(DataType::STRING, data_type_);
  }
}

template <> ObjectLink DataFormat::get<ObjectLink>() {
  if (data_type_ == DataType::OBJECT_LINK) {
    return std::get<ObjectLink>(data_);
  } else {
    throw WrongDataType(DataType::OBJECT_LINK, data_type_);
  }
}

template <> vector<uint8_t> DataFormat::get<vector<uint8_t>>() {
  if (data_type_ == DataType::OPAQUE) {
    return std::get<vector<uint8_t>>(data_);
  } else {
    throw WrongDataType(DataType::OPAQUE, data_type_);
  }
}

} // namespace LwM2M
