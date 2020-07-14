#include "Threadsafe_Queue.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace std;

class ThreadsafeQueueTests : public ::testing::Test {
protected:
  void SetUp() override {
    string_queue = make_shared<ThreadsafeQueue<string>>();
  }
  shared_ptr<ThreadsafeQueue<string>> string_queue;
};

TEST_F(ThreadsafeQueueTests, returnsEmptyPtrOnInstantiation) {
  EXPECT_FALSE(string_queue->try_pop());
  EXPECT_EQ(string_queue->try_pop().get(), nullptr);
}