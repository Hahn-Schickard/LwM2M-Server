#include "DataFormat.hpp"

#include "gtest/gtest.h"

#include <algorithm>

using namespace std;
using namespace LwM2M;

struct DataFormatTestExpectations {
  const DataVariant data;
  const DataType data_type;

  DataFormatTestExpectations(DataVariant data, DataType type)
      : data(move(data)), data_type(type) {}
};

using DataFormatTestExpectationsPtr = shared_ptr<DataFormatTestExpectations>;

struct DataFormatTestParameter {
  DataFormatPtr tested;
  DataFormatTestExpectationsPtr expected;

  DataFormatTestParameter(
      DataFormatPtr tested, DataFormatTestExpectationsPtr expected)
      : tested(move(tested)), expected(move(expected)) {}
};

class DataFormatTest : public testing::TestWithParam<DataFormatTestParameter> {
public:
  void SetUp() override {
    tested = GetParam().tested;
    expected = GetParam().expected;
  }

  DataFormatPtr tested;
  DataFormatTestExpectationsPtr expected;
};

// NOLINTNEXTLINE
TEST_P(DataFormatTest, canGetCorrectValue) {
  try {
    switch (expected->data_type) {
    case DataType::String: {
      tested->get<string>();
      break;
    }
    case DataType::Signed_Integer: {
      tested->get<int64_t>();
      break;
    }
    case DataType::Unsigned_Integer: {
      tested->get<uint64_t>();
      break;
    }
    case DataType::Float: {
      tested->get<double>();
      break;
    }
    case DataType::Boolean: {
      tested->get<bool>();
      break;
    }
    case DataType::Opaque: {
      tested->get<vector<uint8_t>>();
      break;
    }
    case DataType::Time: {
      tested->get<uint64_t>();
      break;
    }
    case DataType::Object_Link: {
      tested->get<ObjectLink>();
      break;
    }
    case DataType::Core_Link: {
      FAIL() << "CoRE Link format is not implemented" << endl;
    }
    case DataType::None: {
      tested->get<void>();
      break;
    }
    }
  } catch (const exception& ex) {
    FAIL() << "Caught exception: " << ex.what() << endl;
  }
}

// NOLINTNEXTLINE
TEST_P(DataFormatTest, UnsupportedDataType) {
  EXPECT_THROW({ tested->get<DataVariant>(); }, UnsupportedDataType);
}

struct GenerateTestName {
  string operator()(
      const testing::TestParamInfo<DataFormatTestParameter>& parameter) const {
    auto name = toString(parameter.param.expected->data_type);
    name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
    return name;
  }
};

DataFormatTestParameter makeTestParameter(
    const DataFormatTestExpectations& valid_expectations) {
  auto data = make_shared<DataFormat>(valid_expectations.data);
  auto expectations =
      make_shared<DataFormatTestExpectations>(valid_expectations);

  return DataFormatTestParameter(move(data), move(expectations));
}

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(DataFormatTests, DataFormatTest,
    testing::Values(makeTestParameter(DataFormatTestExpectations(
                        DataVariant(), DataType::None)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant(string("Hello")), DataType::String)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((int64_t)-10), DataType::Signed_Integer)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((uint64_t)20), DataType::Unsigned_Integer)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((double)30.2), DataType::Float)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((bool)true), DataType::Boolean)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant(vector<uint8_t>{1, 2, 3, 4, 5}), DataType::Opaque)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((uint64_t)2), DataType::Time)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant(ObjectLink(0, 0)), DataType::Object_Link))),
    GenerateTestName());
