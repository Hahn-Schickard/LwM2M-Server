#include "DataFormat.hpp"
#include "Variant_Visitor.hpp"

#include <algorithm>

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

vector<uint8_t> vectorize(uint64_t value, size_t size = sizeof(uint64_t)) {
  vector<uint8_t> result;
  result.reserve(size);
  for (size_t i = 0; i < size; i++) {
    result.push_back(value & 0xFF);
    value = (value >> 8);
  }
  return result;
}

vector<uint8_t> vectorize(double value) {
  uint8_t *bytes = (uint8_t *)&value;
  vector<uint8_t> result;
  result.reserve(sizeof(bytes));
  for (size_t i = 0; i < sizeof(bytes); i++) {
    result.push_back(bytes[i]);
  }
  return result;
}

vector<uint8_t> vectorize(string value) {
  return vector<uint8_t>(value.begin(), value.end());
}

vector<uint8_t> vectorize(ObjectLink value) {
  vector<uint8_t> result =
      vectorize(value.object_id_, sizeof(value.object_id_));
  auto tmp = vectorize(value.instance_id_, sizeof(value.instance_id_));
  result.insert(result.end(), tmp.begin(), tmp.end());
  return result;
}

vector<uint8_t> toBytes(DataVariant data) {
  vector<uint8_t> result;
  match(data, [&](bool value) { result.push_back(value); },
        [&](int64_t value) {
          // possible loss of signess here, but uint64_t (2^64 - 1) should have
          // more spase than an int64_t (±(2^63 - 1)), thus int64_t should never
          // be able to use signess bit as a number
          result = vectorize((uint64_t)value);
        },
        [&](uint64_t value) { result = vectorize(value); },
        [&](double value) { result = vectorize(value); },
        [&](string value) { result = vectorize(value); },
        [&](ObjectLink value) { result = vectorize(value); },
        [&](vector<uint8_t> value) { result = value; });
  return result;
}

uint64_t toInt(vector<uint8_t> bytes, bool little_endian = false) {
  uint64_t result;
  if (little_endian) {
    for (int i = bytes.size(); i >= 0; i--) {
      result = (result << 8) + bytes[i];
    }
  } else {
    for (size_t i = 0; i < bytes.size(); i++) {
      result = (result << 8) + bytes[i];
    }
  }
  return result;
}

DataFormat::DataFormat(vector<uint8_t> bytes) : data_(bytes) {}

DataFormat::DataFormat(DataVariant data) : data_(toBytes(data)) {}

template <> void DataFormat::get<void>() const {}

template <> bool DataFormat::get<bool>() const {
  if (data_.size() == 1) {
    return static_cast<bool>(data_[0]);
  } else {
    throw logic_error("Could not convert to bool");
  }
}

template <> int64_t DataFormat::get<int64_t>() const {
  if (data_.size() <= sizeof(uint64_t)) {
    return (int64_t)toInt(data_);
  } else {
    throw logic_error("Could not convert to int64_t");
  }
}

template <> uint64_t DataFormat::get<uint64_t>() const {
  if (data_.size() <= sizeof(uint64_t)) {
    return toInt(data_);
  } else {
    throw logic_error("Could not convert to uint64_t");
  }
}

template <> double DataFormat::get<double>() const {
  if (data_.size() <= sizeof(double)) {
    double result;
    copy(data_.begin(), data_.end(), reinterpret_cast<char *>(&result));
    return result;
  } else {
    throw logic_error("Could not convert to double");
  }
}

template <> string DataFormat::get<string>() const {
  return string(data_.begin(), data_.end());
}

template <> ObjectLink DataFormat::get<ObjectLink>() const {
  if (data_.size() == sizeof(uint16_t) + sizeof(uint16_t)) {
    vector<uint8_t> object_id_half(data_.begin(),
                                   data_.begin() + data_.size() / 2);
    vector<uint8_t> instance_id_half(data_.begin() + data_.size() / 2,
                                     data_.end());
    uint16_t object_id = toInt(object_id_half);
    uint16_t instance_id = toInt(instance_id_half);
    return ObjectLink(object_id, instance_id);
  } else {
    throw logic_error("Could not convert to ObjectLink");
  }
}

template <> vector<uint8_t> DataFormat::get<vector<uint8_t>>() const {
  return data_;
}

size_t DataFormat::size() const { return data_.size(); }

bool operator==(const DataFormat &lhs, const DataFormat &rhs) {
  return lhs.data_ == rhs.data_ ? true : false;
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
