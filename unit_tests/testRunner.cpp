#include "LoggerRepository.hpp"
#include "gtest/gtest.h"

int main(int argc, char **argv) {
  auto config = HaSLL::Configuration(
      "./log", "tests.log", "[%Y-%m-%d-%H:%M:%S:%F %z][%n]%^[%l]: %v%$",
      HaSLL::SeverityLevel::TRACE, false, 8192, 2, 25, 100, 1);
  HaSLL::LoggerRepository::initialise(config);
  testing::GTEST_FLAG(repeat) = 20;
  testing::GTEST_FLAG(throw_on_failure) = true;

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
