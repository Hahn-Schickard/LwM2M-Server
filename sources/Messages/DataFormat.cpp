#include "DataFormat.hpp"
#include "Variant_Visitor.hpp"

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

DataFormat::DataFormat(DataVariant data, DataType type)
    : data_(data), data_type_(type) {}

template <> void DataFormat::get<void>() const {
  if (data_type_ != DataType::NONE) {
    throw WrongDataType(DataType::NONE, data_type_);
  }
}

template <> bool DataFormat::get<bool>() const {
  if (data_type_ == DataType::BOOLEAN) {
    return std::get<bool>(data_);
  } else {
    throw WrongDataType(DataType::BOOLEAN, data_type_);
  }
}

template <> int64_t DataFormat::get<int64_t>() const {
  if (data_type_ == DataType::SIGNED_INTEGER) {
    return std::get<int64_t>(data_);
  } else {
    throw WrongDataType(DataType::SIGNED_INTEGER, data_type_);
  }
}

template <> uint64_t DataFormat::get<uint64_t>() const {
  if (data_type_ == DataType::UNSIGNED_INTEGER ||
      data_type_ == DataType::TIME) {
    return std::get<uint64_t>(data_);
  } else {
    throw WrongDataType(DataType::UNSIGNED_INTEGER, data_type_);
  }
}

template <> double DataFormat::get<double>() const {
  if (data_type_ == DataType::FLOAT) {
    return std::get<double>(data_);
  } else {
    throw WrongDataType(DataType::FLOAT, data_type_);
  }
}

template <> string DataFormat::get<string>() const {
  if (data_type_ == DataType::STRING) {
    return std::get<string>(data_);
  } else {
    throw WrongDataType(DataType::STRING, data_type_);
  }
}

template <> ObjectLink DataFormat::get<ObjectLink>() const {
  if (data_type_ == DataType::OBJECT_LINK) {
    return std::get<ObjectLink>(data_);
  } else {
    throw WrongDataType(DataType::OBJECT_LINK, data_type_);
  }
}

template <> vector<uint8_t> DataFormat::get<vector<uint8_t>>() const {
  if (data_type_ == DataType::OPAQUE) {
    return std::get<vector<uint8_t>>(data_);
  } else {
    throw WrongDataType(DataType::OPAQUE, data_type_);
  }
}

size_t DataFormat::size() const {
  size_t result = 0;
  match(data_, [&](bool value) { result = sizeof(value); },
        [&](int64_t value) { result = sizeof(value); },
        [&](uint64_t value) { result = sizeof(value); },
        [&](double value) { result = sizeof(value); },
        [&](string value) { result = value.size(); },
        [&](ObjectLink /*value*/) { result = 8; },
        [&](vector<uint8_t> value) { result = value.size(); });

  return result;
}

bool operator==(const DataFormat &lhs, const DataFormat &rhs) {
  if (lhs.data_type_ == rhs.data_type_ && lhs.data_ == rhs.data_) {
    return true;
  } else {
    return false;
  }
}

NotifyAttribute::NotifyAttribute(
    std::optional<uint16_t> minimum_period,
    std::optional<uint16_t> maximum_period, std::optional<double> greater_than,
    std::optional<double> less_than, std::optional<double> step,
    std::optional<uint16_t> minimum_evaluation_period,
    std::optional<uint16_t> maximum_evaluation_period)
    : minimum_period_(minimum_period), maximum_period_(maximum_period),
      greater_than_(greater_than), less_than_(less_than), step_(step),
      minimum_evaluation_period_(minimum_evaluation_period),
      maximum_evaluation_period_(maximum_evaluation_period) {}

size_t NotifyAttribute::size() {
  size_t result = 0;

  if (minimum_period_.has_value()) {
    result += sizeof(minimum_period_.value());
  }
  if (maximum_period_.has_value()) {
    result += sizeof(maximum_period_.value());
  }
  if (greater_than_.has_value()) {
    result += sizeof(greater_than_.value());
  }
  if (less_than_.has_value()) {
    result += sizeof(less_than_.value());
  }
  if (step_.has_value()) {
    result += sizeof(step_.value());
  }
  if (minimum_evaluation_period_.has_value()) {
    result += sizeof(minimum_evaluation_period_.value());
  }
  if (maximum_evaluation_period_.has_value()) {
    result += sizeof(maximum_evaluation_period_.value());
  }

  return result;
}

Payload::Payload(DataFormatPtr data) : Payload(PayloadData(data)) {}

Payload::Payload(TargetContent data) : Payload(PayloadData(data)) {}

Payload::Payload(ElmentIdVariant data) : Payload(PayloadData(data)) {}

Payload::Payload(vector<ElmentIdVariant> data) : Payload(PayloadData(data)) {}

Payload::Payload(vector<TargetContent> data) : Payload(PayloadData(data)) {}

Payload::Payload(vector<TargetAttribute> data) : Payload(PayloadData(data)) {}

Payload::Payload(PayloadData data, MediaType format)
    : data_(data), media_type_(format) {}

size_t size_of(TargetContent value) {
  return size_of(value.first) + value.second->size();
}

size_t size_of(TargetContentVector value) {
  size_t result = 0;
  for (auto target_content : value) {
    result += size_of(target_content);
  }
  return result;
}

size_t Payload::size() {
  size_t result = 0;
  match(data_, [&](DataFormatPtr value) { result = value->size(); },
        [&](TargetContent value) { result = size_of(value); },
        [&](TargetContentVector value) { result = size_of(value); },
        [&](ElmentIdVariant value) { result = size_of(value); },
        [&](vector<ElmentIdVariant> value) {
          for (auto elment_id : value) {
            result += size_of(elment_id);
          }
        },
        [&](vector<TargetAttribute> value) {
          for (auto target_attribute : value) {
            result += size_of(target_attribute.first) +
                      target_attribute.second->size();
          }
        });

  return result;
}
} // namespace LwM2M
