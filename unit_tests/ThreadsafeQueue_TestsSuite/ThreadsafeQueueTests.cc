#include "Threadsafe_Queue.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace std;

class ThreadsafeQueueTests : public ::testing::Test {
protected:
  void SetUp() override {
    string_queue = make_shared<ThreadsafeQueue<string>>();
    int_queue = make_shared<ThreadsafeQueue<int>>();
  }
  shared_ptr<ThreadsafeQueue<string>> string_queue;
  shared_ptr<ThreadsafeQueue<int>> int_queue;
};

TEST_F(ThreadsafeQueueTests, returnsEmptyPtrOnInstantiation) {
  EXPECT_FALSE(string_queue->try_pop());
  EXPECT_EQ(string_queue->try_pop().get(), nullptr);
}
TEST_F(ThreadsafeQueueTests, canPopValueAsAReference) {
  string expected = "Hello World!";
  EXPECT_NO_THROW(string_queue->push(expected));
  string tested;
  EXPECT_TRUE(string_queue->try_pop(tested));
  EXPECT_EQ(tested, expected);
}

TEST_F(ThreadsafeQueueTests, canPopValue) {
  int expected = 42;
  EXPECT_NO_THROW(int_queue->push(expected));
  EXPECT_EQ(*int_queue->wait_and_pop().get(), expected);
}