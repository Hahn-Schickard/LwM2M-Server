#include "Register.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace LwM2M;

TEST(RegisterResponseTests, throwsUnsupportedResponseCode) {
  EXPECT_THROW(RegisterResponse(make_shared<Endpoint>("0.0.0.0", 86567),
                                ResponseCode::OK, string()),
               UnsupportedResponseCode);
}
