#include "Threadsafe_Unique_Queue.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace std;

class ThreadsafeUniqueQueueWithIntegersTests : public ::testing::Test {
protected:
  void SetUp() override { queue = make_unique<ThreadsafeUniqueQueue<int>>(); }
  unique_ptr<ThreadsafeUniqueQueue<int>> queue;
};

TEST_F(ThreadsafeUniqueQueueWithIntegersTests, returnsEmptyPtrForEmptyQueue) {
  EXPECT_FALSE(queue->try_pop());
  EXPECT_EQ(queue->try_pop().get(), nullptr);
}

TEST_F(ThreadsafeUniqueQueueWithIntegersTests, canPushNewInteger) {
  int expected = 42;
  EXPECT_NO_THROW(queue->push(expected));
  int tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested, expected);
}
