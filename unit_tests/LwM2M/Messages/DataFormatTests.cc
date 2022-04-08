#include "DataFormat.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace LwM2M;

struct DataFormatTestExpectations {
  const DataVariant data_;
  const DataType data_type_;

  DataFormatTestExpectations(DataVariant data, DataType type)
      : data_(data), data_type_(type) {}
};

using DataFormatTestExpectationsPtr = shared_ptr<DataFormatTestExpectations>;

struct DataFormatTestParameter {
  DataFormatPtr tested_;
  DataFormatTestExpectationsPtr expected_;

  DataFormatTestParameter(DataFormatPtr tested,
                          DataFormatTestExpectationsPtr expected)
      : tested_(tested), expected_(expected) {}
};

class DataFormatTest : public testing::TestWithParam<DataFormatTestParameter> {
public:
  void SetUp() override {
    tested_ = GetParam().tested_;
    expected_ = GetParam().expected_;
  }

  DataFormatPtr tested_;
  DataFormatTestExpectationsPtr expected_;
};

TEST_P(DataFormatTest, canGetCorrectValue) {
  try {
    switch (expected_->data_type_) {
    case DataType::STRING: {
      tested_->get<string>();
      break;
    }
    case DataType::SIGNED_INTEGER: {
      tested_->get<int64_t>();
      break;
    }
    case DataType::UNSIGNED_INTEGER: {
      tested_->get<uint64_t>();
      break;
    }
    case DataType::FLOAT: {
      tested_->get<double>();
      break;
    }
    case DataType::BOOLEAN: {
      tested_->get<bool>();
      break;
    }
    case DataType::OPAQUE: {
      tested_->get<vector<uint8_t>>();
      break;
    }
    case DataType::TIME: {
      tested_->get<uint64_t>();
      break;
    }
    case DataType::OBJECT_LINK: {
      tested_->get<ObjectLink>();
      break;
    }
    case DataType::CORE_LINK: {
      FAIL() << "CoRE Link format is not implemented" << endl;
    }
    case DataType::NONE: {
      tested_->get<void>();
      break;
    }
    }
  } catch (exception &ex) {
    FAIL() << "Caught exception: " << ex.what() << endl;
  }
}

TEST_P(DataFormatTest, UnsupportedDataType) {
  EXPECT_THROW({ tested_->get<DataVariant>(); }, UnsupportedDataType);
}

struct GenerateTestName {
  string operator()(
      const testing::TestParamInfo<DataFormatTestParameter> &parameter) const {
    auto name = toString(parameter.param.expected_->data_type_);
    name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
    return name;
  }
};

DataFormatTestParameter
makeTestParameter(const DataFormatTestExpectations &valid_expectations) {
  auto data = make_shared<DataFormat>(valid_expectations.data_);
  auto expectations =
      make_shared<DataFormatTestExpectations>(valid_expectations);

  return DataFormatTestParameter(move(data), move(expectations));
}

INSTANTIATE_TEST_SUITE_P(
    DataFormatTests, DataFormatTest,
    testing::Values(
        makeTestParameter(DataFormatTestExpectations(DataVariant(),
                                                     DataType::NONE)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant(string("Hello")), DataType::STRING)),
        makeTestParameter(DataFormatTestExpectations(DataVariant((int64_t)-10),
                                                     DataType::SIGNED_INTEGER)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant((uint64_t)20), DataType::UNSIGNED_INTEGER)),
        makeTestParameter(DataFormatTestExpectations(DataVariant((double)30.2),
                                                     DataType::FLOAT)),
        makeTestParameter(DataFormatTestExpectations(DataVariant((bool)true),
                                                     DataType::BOOLEAN)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant(vector<uint8_t>{1, 2, 3, 4, 5}), DataType::OPAQUE)),
        makeTestParameter(DataFormatTestExpectations(DataVariant((uint64_t)2),
                                                     DataType::TIME)),
        makeTestParameter(DataFormatTestExpectations(
            DataVariant(ObjectLink(0, 0)), DataType::OBJECT_LINK))),
    GenerateTestName());