#include "Register.hpp"

#include "gtest/gtest.h"

using namespace std;
using namespace LwM2M;

// NOLINTNEXTLINE
TEST(RegisterResponseTests, throwsUnsupportedResponseCode) {
  EXPECT_THROW(RegisterResponse(
                   make_shared<Endpoint>("0.0.0.0", 86567), ResponseCode::OK),
      UnsupportedResponseCode);
}
