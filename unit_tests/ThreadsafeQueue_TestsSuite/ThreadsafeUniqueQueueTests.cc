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

TEST_F(ThreadsafeUniqueQueueWithIntegersTests, canPushNewValue) {
  int expected = 42;
  EXPECT_NO_THROW(queue->push(expected));
  int tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested, expected);
}

TEST_F(ThreadsafeUniqueQueueWithIntegersTests, canNotPushDuplicateValue) {
  int expected = 78;
  EXPECT_NO_THROW(queue->push(expected));
  EXPECT_NO_THROW(queue->push(expected));
  EXPECT_NO_THROW(queue->push(12));
  int tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested, expected);
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_NE(tested, expected);
}

class ThreadsafeUniqueQueueWithStringsTests : public ::testing::Test {
protected:
  void SetUp() override {
    queue = make_unique<ThreadsafeUniqueQueue<string>>();
  }
  unique_ptr<ThreadsafeUniqueQueue<string>> queue;
};

TEST_F(ThreadsafeUniqueQueueWithStringsTests, returnsEmptyPtrForEmptyQueue) {
  EXPECT_FALSE(queue->try_pop());
  EXPECT_EQ(queue->try_pop().get(), nullptr);
}

TEST_F(ThreadsafeUniqueQueueWithStringsTests, canPushNewValue) {
  string expected = "Hello";
  EXPECT_NO_THROW(queue->push(expected));
  string tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested, expected);
}

TEST_F(ThreadsafeUniqueQueueWithStringsTests, canNotPushDuplicateValue) {
  string expected = "World";
  EXPECT_NO_THROW(queue->push(expected));
  EXPECT_NO_THROW(queue->push(expected));
  EXPECT_NO_THROW(queue->push(string("Bob")));
  string tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested, expected);
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_NE(tested, expected);
}

TEST_F(ThreadsafeUniqueQueueWithStringsTests, canPushUniquePtr) {
  string expected = "Hello";
  EXPECT_NO_THROW(queue->push(make_unique<string>(expected)));
  string tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested, expected);
}

TEST_F(ThreadsafeUniqueQueueWithStringsTests, canPophUniquePtr) {
  string expected = "Hello";
  EXPECT_NO_THROW(queue->push(expected));
  unique_ptr<string> tested;
  EXPECT_TRUE(tested = queue->try_pop());
  EXPECT_EQ(*tested, expected);
}

TEST_F(ThreadsafeUniqueQueueWithStringsTests, canPushAndPophUniquePtr) {
  string expected = "Hello";
  EXPECT_NO_THROW(queue->push(make_unique<string>(expected)));
  unique_ptr<string> tested;
  EXPECT_TRUE(tested = queue->try_pop());
  EXPECT_EQ(*tested, expected);
}

TEST_F(ThreadsafeUniqueQueueWithStringsTests,
       canNotPushUniquePtrWithDuplicateValue) {
  string expected = "World";
  EXPECT_NO_THROW(queue->push(make_unique<string>(expected)));
  EXPECT_NO_THROW(queue->push(make_unique<string>(expected)));
  EXPECT_NO_THROW(queue->push(string("Bob")));
  string tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested, expected);
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_NE(tested, expected);
}

struct Custom {
  string name;
  int age;
};

namespace std {
template <> struct hash<Custom> {
public:
  size_t operator()(const Custom &value) const {
    return hash<string>{}(value.name) + hash<int>{}(value.age);
  }
};

bool operator==(const Custom &lhs, const Custom &rhs) {
  return (lhs.name == rhs.name && lhs.age == rhs.age);
}
} // namespace std

class ThreadsafeUniqueQueueWithCustomClassTests : public ::testing::Test {
protected:
  void SetUp() override {
    queue = make_unique<ThreadsafeUniqueQueue<Custom>>();
  }
  unique_ptr<ThreadsafeUniqueQueue<Custom>> queue;
};

TEST_F(ThreadsafeUniqueQueueWithCustomClassTests,
       returnsEmptyPtrForEmptyQueue) {
  EXPECT_FALSE(queue->try_pop());
  EXPECT_EQ(queue->try_pop().get(), nullptr);
}

TEST_F(ThreadsafeUniqueQueueWithCustomClassTests, canPushNewValue) {
  Custom expected{"Bob", 22};
  EXPECT_NO_THROW(queue->push(expected));
  Custom tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested.age, expected.age);
  EXPECT_EQ(tested.name, expected.name);
}

TEST_F(ThreadsafeUniqueQueueWithCustomClassTests, canNotPushDuplicateValue) {
  Custom expected{"Jane", 23};
  EXPECT_NO_THROW(queue->push(expected));
  EXPECT_NO_THROW(queue->push(expected));
  EXPECT_NO_THROW(queue->push(Custom{"Scarlet", 19}));
  Custom tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested.age, expected.age);
  EXPECT_EQ(tested.name, expected.name);
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_NE(tested.age, expected.age);
  EXPECT_NE(tested.name, expected.name);
}

TEST_F(ThreadsafeUniqueQueueWithCustomClassTests, canPushUniquePtr) {
  Custom expected{"Wane", 30};
  EXPECT_NO_THROW(queue->push(make_unique<Custom>(expected)));
  Custom tested;
  EXPECT_TRUE(queue->try_pop(tested));
  EXPECT_EQ(tested.age, expected.age);
  EXPECT_EQ(tested.name, expected.name);
}

TEST_F(ThreadsafeUniqueQueueWithCustomClassTests, canPophUniquePtr) {
  Custom expected{"Barbara", 34};
  EXPECT_NO_THROW(queue->push(expected));
  unique_ptr<Custom> tested;
  EXPECT_TRUE(tested = queue->try_pop());
  EXPECT_EQ(tested->age, expected.age);
  EXPECT_EQ(tested->name, expected.name);
}

TEST_F(ThreadsafeUniqueQueueWithCustomClassTests, canPushAndPophUniquePtr) {
  Custom expected{"Robin", 18};
  EXPECT_NO_THROW(queue->push(make_unique<Custom>(expected)));
  unique_ptr<Custom> tested;
  EXPECT_TRUE(tested = queue->try_pop());
  EXPECT_EQ(tested->age, expected.age);
  EXPECT_EQ(tested->name, expected.name);
}

TEST_F(ThreadsafeUniqueQueueWithCustomClassTests,
       canNotPushUniquePtrWithDuplicateValue) {
  Custom expected{"Joker", 40};
  EXPECT_NO_THROW(queue->push(make_unique<Custom>(expected)));
  EXPECT_NO_THROW(queue->push(make_unique<Custom>(expected)));
  EXPECT_NO_THROW(queue->push(Custom{"Sage", 60}));
  unique_ptr<Custom> tested;
  EXPECT_TRUE(tested = queue->try_pop());
  EXPECT_EQ(tested->age, expected.age);
  EXPECT_EQ(tested->name, expected.name);
  EXPECT_TRUE(tested = queue->try_pop());
  EXPECT_NE(tested->age, expected.age);
  EXPECT_NE(tested->name, expected.name);
}