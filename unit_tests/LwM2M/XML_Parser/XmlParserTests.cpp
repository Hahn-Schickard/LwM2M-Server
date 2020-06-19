#include "XmlParser.hpp"

#include "gtest/gtest.h"
#include <vector>

using namespace std;
using namespace LwM2M_Model;

TEST(XmlParserTests, canDeserializeModel) {
  ASSERT_NO_THROW({
    vector<LwM2M_Object> lwm2m_model =
        deserializeModel("unit_tests/LwM2M/XML_Parser/model/ipsoConfig.xml");
  });
}