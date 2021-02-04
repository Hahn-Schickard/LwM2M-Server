#include "DataFormat.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace LwM2M;

struct TestExpectations {
  const DataVariant data_;
  const DataType data_type_;
  const MediaType media_type_;
};

using TestExpectationsPtr = shared_ptr<TestExpectations>;

struct TestParameter {
  DataFormatPtr tested_;
  TestExpectationsPtr expected_;

  TestParameter(DataFormatPtr tested, TestExpectationsPtr expected)
      : tested_(tested), expected_(expected) {}
};

class DataFormatTest : public testing::TestWithParam<TestParameter> {
public:
  void SetUp() override {
    tested_ = GetParam().tested_;
    expected_ = GetParam().expected_;
  }

  DataFormatPtr tested_;
  TestExpectationsPtr expected_;
};

TEST_P(DataFormatTest, returnsCorrectDataType) {
  EXPECT_EQ(tested_->data_type_, expected_->data_type_);
}

TEST_P(DataFormatTest, returnsCorrectMediaType) {
  EXPECT_EQ(tested_->media_type_, expected_->media_type_);
}

TEST_P(DataFormatTest, contaisCorrectData) {
  EXPECT_EQ(tested_->data_, expected_->data_);
}

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
    FAIL() << "cought exception: " << ex.what() << endl;
  }
}

TEST_P(DataFormatTest, throwsWrongDataType) {
  EXPECT_THROW(
      {
        switch (expected_->data_type_) {
        case DataType::STRING: {
          tested_->get<int64_t>();
          break;
        }
        case DataType::SIGNED_INTEGER: {
          tested_->get<string>();
          break;
        }
        case DataType::UNSIGNED_INTEGER: {
          tested_->get<void>();
          break;
        }
        case DataType::FLOAT: {
          tested_->get<bool>();

          break;
        }
        case DataType::BOOLEAN: {
          tested_->get<double>();
          break;
        }
        case DataType::OPAQUE: {
          tested_->get<ObjectLink>();
          break;
        }
        case DataType::TIME: {
          tested_->get<string>();
          break;
        }
        case DataType::OBJECT_LINK: {

          tested_->get<vector<uint8_t>>();
          break;
        }
        case DataType::CORE_LINK: {
          FAIL() << "CoRE Link format is not implemented" << endl;
        }
        case DataType::NONE: {
          tested_->get<uint64_t>();
          break;
        }
        }
      },
      WrongDataType);
}

TEST_P(DataFormatTest, UnsuportedDataType) {
  EXPECT_THROW({ tested_->get<DataVariant>(); }, UnsuportedDataType);
}

struct GenerateTestName {
  string
  operator()(const testing::TestParamInfo<TestParameter> &parameter) const {
    auto name = toString(parameter.param.tested_->media_type_) +
                toString(parameter.param.tested_->data_type_);
    name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
    return name;
  }
};

TestParameter makeTestParameter(const TestExpectations &valid_expectations) {
  auto data = make_shared<DataFormat>(valid_expectations.data_,
                                      valid_expectations.data_type_,
                                      valid_expectations.media_type_);
  auto expectations = make_shared<TestExpectations>(valid_expectations);

  return TestParameter(move(data), move(expectations));
}

INSTANTIATE_TEST_SUITE_P(
    DataFormatTests, DataFormatTest,
    testing::Values(makeTestParameter(TestExpectations{
                        .data_ = DataVariant(),
                        .data_type_ = DataType::NONE,
                        .media_type_ = MediaType::NOT_SPECIFIED}),
                    makeTestParameter(TestExpectations{
                        .data_ = DataVariant(string("Hello")),
                        .data_type_ = DataType::STRING,
                        .media_type_ = MediaType::NOT_SPECIFIED}),
                    makeTestParameter(TestExpectations{
                        .data_ = DataVariant((int64_t)-10),
                        .data_type_ = DataType::SIGNED_INTEGER,
                        .media_type_ = MediaType::NOT_SPECIFIED}),
                    makeTestParameter(TestExpectations{
                        .data_ = DataVariant((uint64_t)20),
                        .data_type_ = DataType::UNSIGNED_INTEGER,
                        .media_type_ = MediaType::NOT_SPECIFIED}),
                    makeTestParameter(TestExpectations{
                        .data_ = DataVariant((double)30.2),
                        .data_type_ = DataType::FLOAT,
                        .media_type_ = MediaType::NOT_SPECIFIED}),
                    makeTestParameter(TestExpectations{
                        .data_ = DataVariant((bool)true),
                        .data_type_ = DataType::BOOLEAN,
                        .media_type_ = MediaType::NOT_SPECIFIED}),
                    makeTestParameter(TestExpectations{
                        .data_ = DataVariant(vector<uint8_t>{1, 2, 3, 4, 5}),
                        .data_type_ = DataType::OPAQUE,
                        .media_type_ = MediaType::NOT_SPECIFIED}),
                    makeTestParameter(TestExpectations{
                        .data_ = DataVariant((uint64_t)2),
                        .data_type_ = DataType::TIME,
                        .media_type_ = MediaType::NOT_SPECIFIED}),
                    makeTestParameter(TestExpectations{
                        .data_ = DataVariant(ObjectLink(0, 0)),
                        .data_type_ = DataType::OBJECT_LINK,
                        .media_type_ = MediaType::NOT_SPECIFIED})),
    GenerateTestName());