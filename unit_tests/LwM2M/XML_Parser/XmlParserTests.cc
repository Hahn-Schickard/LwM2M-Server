#include "XmlParser.hpp"

#include "gtest/gtest.h"
#include <vector>

using namespace std;
using namespace LwM2M;

TEST(XmlParserTests, canDeserializeModel) {
  EXPECT_NO_THROW(deserializeModel("model/passingModel1.xml"));
}

TEST(XmlParserTests, thorwsExceptionOnNonExistantConfigFile) {
  EXPECT_THROW(deserializeModel("model/doesNotExist.xml"), runtime_error);
}

TEST(XmlParserTests, thorwsExceptionOnNonExistantDescriptor) {
  EXPECT_THROW(deserializeModel("model/faillingModel1.xml"), runtime_error);
}

TEST(XmlParserTests, canDeserializeDeviceObject) {
  unordered_map<uint32_t, ObjectDescriptor> model;
  EXPECT_NO_THROW({ model = deserializeModel("model/passingModel2.xml"); });

  EXPECT_EQ(model.size(), 1);

  ObjectDescriptor device_object = model[3];
  EXPECT_EQ(device_object.id_, 3);
  EXPECT_EQ(device_object.name_, "Device");
  EXPECT_EQ(device_object.urn_, "urn:oma:lwm2m:oma:3");
  EXPECT_EQ(device_object.multiple_instances_, false);
  EXPECT_EQ(device_object.mandatory_, true);
  EXPECT_EQ(device_object.resources_.size(), 23);

  for (uint8_t expected_id = 0; expected_id < 23; expected_id++) {
    auto resource = device_object.resources_.at(expected_id);
    EXPECT_EQ(resource.id_, expected_id);
  }
}