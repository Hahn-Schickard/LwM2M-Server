#include "DataFormat.hpp"
#include "HSCUL/ByteArray.hpp"
#include "HSCUL/FloatingPoint.hpp"
#include "HSCUL/Integer.hpp"
#include "HSCUL/String.hpp"
#include "Variant_Visitor.hpp"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace HSCUL;

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
  default: {
    return "Not specified";
  }
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
  default: {
    return "None";
  }
  }
}

UnsupportedDataType::UnsupportedDataType()
    : logic_error("Tried to access unhandled data type!") {}

TimeStamp::TimeStamp(uint64_t posix_timestamp) : value_(posix_timestamp) {}

string TimeStamp::toString() {
  tm* tm = localtime(&value_);
  stringstream ss;
  ss << put_time(tm, "%F %T %Z");
  return ss.str();
}

time_t TimeStamp::getValue() const { return value_; }

vector<uint8_t> toBytes(const DataVariant& data) {
  vector<uint8_t> result;
  match(
      data, [&](bool value) { result.push_back(value); }, // NOLINT
      [&](int64_t value) {
        result = HSCUL::toBytes(value, ByteOrder::NetworkByteOrder);
      },
      [&](uint64_t value) {
        result = HSCUL::toBytes(value, ByteOrder::NetworkByteOrder);
      },
      [&](double value) {
        result = HSCUL::toBytes(value, ByteOrder::NetworkByteOrder);
      },
      [&](const string& value) { result = HSCUL::toBytes(value); },
      [&](TimeStamp value) {
        uint64_t converted = value.getValue();
        result = HSCUL::toBytes(converted, ByteOrder::NetworkByteOrder);
      },
      [&](ObjectLink value) {
        result = HSCUL::toBytes(value.object_id_, ByteOrder::NetworkByteOrder);
        auto expansion =
            HSCUL::toBytes(value.instance_id_, ByteOrder::NetworkByteOrder);
        result.insert(result.end(), expansion.begin(), expansion.end());
      },
      [&](vector<uint8_t> value) { result = value; });
  return result;
}

DataFormat::DataFormat(const vector<uint8_t>& bytes) : data_(bytes) {} // NOLINT

DataFormat::DataFormat(const DataVariant& data) : data_(toBytes(data)) {}

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
    return HSCUL::toSignedInteger(data_, ByteOrder::NetworkByteOrder);
  } else {
    throw logic_error("Could not convert to int64_t");
  }
}

template <> uint64_t DataFormat::get<uint64_t>() const {
  if (data_.size() <= sizeof(uint64_t)) {
    return HSCUL::toUnsignedInteger(data_, ByteOrder::NetworkByteOrder);
  } else {
    throw logic_error("Could not convert to uint64_t");
  }
}

template <> TimeStamp DataFormat::get<TimeStamp>() const {
  if (data_.size() <= sizeof(uint64_t)) {
    auto posix_time = get<uint64_t>();
    return TimeStamp(posix_time);
  } else {
    throw logic_error("Could not convert to uint64_t");
  }
}

template <> double DataFormat::get<double>() const {
  if (data_.size() <= sizeof(double)) {
    return HSCUL::toDouble(data_, ByteOrder::NetworkByteOrder);
  } else {
    throw logic_error("Could not convert to double");
  }
}

template <> string DataFormat::get<string>() const {
  return string(data_.begin(), data_.end());
}

template <> ObjectLink DataFormat::get<ObjectLink>() const {
  if (data_.size() >= 2 &&
      data_.size() <= sizeof(uint16_t) + sizeof(uint16_t)) {
    vector<uint8_t> object_id_half(
        data_.begin(), data_.begin() + data_.size() / 2);
    vector<uint8_t> instance_id_half(
        data_.begin() + data_.size() / 2, data_.end());
    uint16_t object_id =
        HSCUL::toUnsignedInteger(object_id_half, ByteOrder::NetworkByteOrder);
    uint16_t instance_id =
        HSCUL::toUnsignedInteger(instance_id_half, ByteOrder::NetworkByteOrder);
    return ObjectLink(object_id, instance_id);
  } else {
    throw logic_error("Could not convert to ObjectLink");
  }
}

template <> vector<uint8_t> DataFormat::get<vector<uint8_t>>() const {
  return data_;
}

DataVariant DataFormat::get(const DataType type) const {
  switch (type) {
  case DataType::BOOLEAN: {
    return DataVariant(get<bool>());
  }
  case DataType::SIGNED_INTEGER: {
    return DataVariant(get<int64_t>());
  }
  case DataType::UNSIGNED_INTEGER: {
    return DataVariant(get<uint64_t>());
  }
  case DataType::FLOAT: {
    return DataVariant(get<double>());
  }
  case DataType::STRING: {
    return DataVariant(get<string>());
  }
  case DataType::TIME: {
    return DataVariant(get<TimeStamp>());
  }
  case DataType::OBJECT_LINK: {
    return DataVariant(get<ObjectLink>());
  }
  case DataType::OPAQUE: {
    return DataVariant(get<vector<uint8_t>>());
  }
  default: {
    auto error_msg =
        "Given data type " + toString(type) + " can not be converted";
    throw invalid_argument(error_msg);
  }
  }
}

size_t DataFormat::size() const { return data_.size(); }

bool operator==(const DataFormat& lhs, const DataFormat& rhs) {
  return lhs.data_ == rhs.data_;
}

NotifyAttribute::NotifyAttribute(std::optional<uint16_t> minimum_period,
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

string NotifyAttribute::toString() {
  string result;

  if (minimum_period_.has_value()) {
    result += "pmin=" + to_string(minimum_period_.value());
  }
  if (maximum_period_.has_value()) {
    result += "pmax=" + to_string(maximum_period_.value());
  }
  if (greater_than_.has_value()) {
    result += "gt=" + to_string(greater_than_.value());
  }
  if (less_than_.has_value()) {
    result += "lt=" + to_string(less_than_.value());
  }
  if (step_.has_value()) {
    result += "st=" + to_string(step_.value());
  }
  if (minimum_evaluation_period_.has_value()) {
    result += "epmin=" + to_string(minimum_evaluation_period_.value());
  }
  if (maximum_evaluation_period_.has_value()) {
    result += "epmax=" + to_string(maximum_evaluation_period_.value());
  }

  return result;
}

Payload::Payload(DataFormatPtr data) : Payload(PayloadData(data)) {}

Payload::Payload(TargetContent data) : Payload(PayloadData(data)) {}

Payload::Payload(ElementID data) : Payload(PayloadData(data)) {}

Payload::Payload(ElementIDs data) : Payload(PayloadData(data)) {}

Payload::Payload(vector<TargetContent> data) : Payload(PayloadData(data)) {}

Payload::Payload(vector<TargetAttribute> data) : Payload(PayloadData(data)) {}

Payload::Payload(PayloadData data, MediaType format) // NOLINT
    : data_(data), media_type_(format) {} // NOLINT

size_t size_of(const TargetContent& value) {
  return value.first.size() + value.second->size();
}

size_t size_of(const TargetAttribute& value) {
  return value.first.size() + value.second->size();
}

size_t size_of(const TargetContentVector& value) {
  size_t result = 0;
  for (const auto& target_content : value) {
    result += size_of(target_content);
  }
  return result;
}

size_t size_of(const PayloadData& data) {
  size_t result = 0;
  match(
      data,
      [&](const DataFormatPtr& value) {
        if (value) {
          result = value->size();
        }
      },
      [&](TargetContent value) { result = size_of(value); },
      [&](TargetContentVector value) { result = size_of(value); },
      [&](const ElementID& value) { result = value.size(); },
      [&](ElementIDs value) {
        for (auto element_id : value) {
          result += element_id.size();
        }
      },
      [&](vector<TargetAttribute> value) {
        for (const auto& target_attribute : value) {
          result += size_of(target_attribute);
        }
      });
  return result;
}

size_t Payload::size() { return size_of(data_); }

string Payload::dataType() {
  string result;
  match(
      data_, [&](DataFormatPtr /*value*/) { result = "Data Variant"; },
      [&](TargetContent /*value*/) { result = "Target Content"; },
      [&](TargetContentVector /*value*/) { result = "Target Content Vector"; },
      [&](ElementID /*value*/) { result = "Element ID"; },
      [&](ElementIDs /*value*/) { result = "Element ID vector"; },
      [&](vector<TargetAttribute> /*value*/) {
        result = "Target Attribute Vector";
      });

  return result;
}

bool Payload::hasData() {
  bool result = false;
  match(
      data_, [&](DataFormatPtr value) { result = static_cast<bool>(value); },
      [&](TargetContent value) {
        result = (value.first.size() != 0) && (static_cast<bool>(value.second));
      },
      [&](TargetContentVector value) { result = !(value.empty()); },
      [&](ElementID value) { result = value.size() != 0; },
      [&](ElementIDs value) { result = !(value.empty()); },
      [&](vector<TargetAttribute> value) { result = !(value.empty()); });

  return result;
}
} // namespace LwM2M

size_t std::hash<LwM2M::DataFormat>::operator()(
    const LwM2M::DataFormat& data) const {
  size_t hash_value = 0;
  for (auto byte : data.get<std::vector<uint8_t>>()) {
    hash_value |= byte << 8;
  }
  return hash_value;
}

size_t std::hash<LwM2M::TargetContent>::operator()(
    const LwM2M::TargetContent& data) const {
  size_t hash_value = 0;
  hash_value |= hash<LwM2M::ElementID>{}(data.first);
  hash_value <<= data.first.size();
  hash_value |= hash<LwM2M::DataFormat>{}(*(data.second));
  return hash_value;
}

size_t std::hash<LwM2M::NotifyAttribute>::operator()(
    const LwM2M::NotifyAttribute& data) const {
  size_t hash_value = 0;

  if (data.minimum_period_) {
    hash_value |= data.minimum_period_.value()
        << sizeof(data.minimum_period_.value());
  }
  if (data.maximum_period_) {
    hash_value |= data.maximum_period_.value()
        << sizeof(data.maximum_period_.value());
  }
  if (data.greater_than_) {
    hash_value |= hash<double>{}(data.greater_than_.value())
        << sizeof(data.greater_than_.value());
  }
  if (data.less_than_) {
    hash_value |= hash<double>{}(data.less_than_.value())
        << sizeof(data.less_than_.value());
  }
  if (data.step_) {
    hash_value |= hash<double>{}(data.step_.value())
        << sizeof(data.step_.value());
  }
  if (data.minimum_evaluation_period_) {
    hash_value |= data.minimum_evaluation_period_.value()
        << sizeof(data.minimum_evaluation_period_.value());
  }
  if (data.maximum_evaluation_period_) {
    hash_value |= data.maximum_evaluation_period_.value()
        << sizeof(data.maximum_evaluation_period_.value());
  }

  return hash_value;
}

size_t std::hash<LwM2M::TargetAttribute>::operator()(
    const LwM2M::TargetAttribute& data) const {
  size_t hash_value = 0;
  hash_value |= hash<LwM2M::ElementID>{}(data.first);
  hash_value <<= data.first.size();
  hash_value |= hash<LwM2M::NotifyAttribute>{}(*(data.second));
  return hash_value;
}

size_t std::hash<LwM2M::PayloadData>::operator()(
    const LwM2M::PayloadData& data) const {
  size_t hash_value = 0;
  match(
      data,
      [&](LwM2M::DataFormatPtr value) {
        hash_value = hash<LwM2M::DataFormat>{}(*value);
      },
      [&](LwM2M::TargetContent value) {
        hash_value = hash<LwM2M::TargetContent>{}(value);
      },
      [&](LwM2M::TargetContentVector value) {
        for (const auto& target_value : value) {
          hash_value |= hash<LwM2M::TargetContent>{}(target_value)
              << size_of(target_value);
        }
      },
      [&](LwM2M::ElementID value) {
        hash_value = hash<LwM2M::ElementID>{}(value);
      },
      [&](LwM2M::ElementIDs value) {
        for (const auto& elements_id : value) {
          hash_value |= hash<LwM2M::ElementID>{}(elements_id)
              << elements_id.size();
        }
      },
      [&](vector<LwM2M::TargetAttribute> value) {
        for (const auto& target_attribute : value) {
          hash_value |= hash<LwM2M::TargetAttribute>{}(target_attribute)
              << size_of(target_attribute);
        }
      });
  return hash_value;
}

size_t std::hash<LwM2M::Payload>::operator()(
    const LwM2M::Payload& payload) const {
  auto result = hash<LwM2M::PayloadData>{}(payload.data_);
  result <<= LwM2M::size_of(payload.data_);
  result |= static_cast<size_t>(payload.media_type_);
  return result;
}