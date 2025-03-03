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
    case DataType::STRING: {
      tested->get<string>();
      break;
    }
    case DataType::SIGNED_INTEGER: {
      tested->get<int64_t>();
      break;
    }
    case DataType::UNSIGNED_INTEGER: {
      tested->get<uint64_t>();
      break;
    }
    case DataType::FLOAT: {
      tested->get<double>();
      break;
    }
    case DataType::BOOLEAN: {
      tested->get<bool>();
      break;
    }
    case DataType::OPAQUE: {
      tested->get<vector<uint8_t>>();
      break;
    }
    case DataType::TIME: {
      tested->get<uint64_t>();
      break;
    }
    case DataType::OBJECT_LINK: {
      tested->get<ObjectLink>();
      break;
    }
    case DataType::CORE_LINK: {
      FAIL() << "CoRE Link format is not implemented" << endl;
    }
    case DataType::NONE: {
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
                        DataVariant(), DataType::NONE)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant(string("Hello")), DataType::STRING)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((int64_t)-10), DataType::SIGNED_INTEGER)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((uint64_t)20), DataType::UNSIGNED_INTEGER)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((double)30.2), DataType::FLOAT)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((bool)true), DataType::BOOLEAN)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant(vector<uint8_t>{1, 2, 3, 4, 5}), DataType::OPAQUE)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((uint64_t)2), DataType::TIME)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant(ObjectLink(0, 0)), DataType::OBJECT_LINK))),
    GenerateTestName());
