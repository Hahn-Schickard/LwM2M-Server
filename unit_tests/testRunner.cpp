#include "HaSLL/LoggerManager.hpp"

#include "gtest/gtest.h"

using namespace HaSLL;

int main(int argc, char** argv) {
  LoggerManager::initialise(makeDefaultRepository());
  testing::GTEST_FLAG(throw_on_failure) = true;

  testing::InitGoogleTest(&argc, argv);
  auto status = RUN_ALL_TESTS();

  LoggerManager::terminate();
  return status;
}
