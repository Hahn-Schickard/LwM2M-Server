#include "Threadsafe_HashSet.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace std;

class ThreadsafeHashSetWithIntegersTests : public ::testing::Test {
protected:
  void SetUp() override { hash_set = make_unique<ThreadsafeHashSet<int>>(); }
  unique_ptr<ThreadsafeHashSet<int>> hash_set;
};

TEST_F(ThreadsafeHashSetWithIntegersTests, returnsEmptyPtrForEmptyHash_set) {
  EXPECT_FALSE(hash_set->front());
}

TEST_F(ThreadsafeHashSetWithIntegersTests, canPushNewValue) {
  int expected = 42;
  EXPECT_NO_THROW(hash_set->push(expected));
  shared_ptr<int> tested = hash_set->front();
  EXPECT_EQ(*tested.get(), expected);
  hash_set->pop(tested);
  EXPECT_TRUE(hash_set->empty());
}

TEST_F(ThreadsafeHashSetWithIntegersTests, canNotPushDuplicateValue) {
  int expected = 78;
  EXPECT_NO_THROW(hash_set->push(expected));
  EXPECT_NO_THROW(hash_set->push(expected));
  EXPECT_NO_THROW(hash_set->push(12));
  EXPECT_EQ(2, hash_set->size());
  shared_ptr<int> tested = hash_set->front();
  EXPECT_EQ(*tested.get(), expected);
  hash_set->pop(tested);
  tested = hash_set->front();
  EXPECT_NE(*tested.get(), expected);
  hash_set->pop(tested);
  EXPECT_TRUE(hash_set->empty());
}