#include "ObserverPatterTestsShared.hpp"
#include "Observer_Pattern.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using namespace std;
using namespace ObserverPattern;

class RegistrationTableMock : public EventSourceInterface<PersonEvent> {
public:
  MOCK_METHOD(void, attach, (EventListenerPtr<PersonEvent>), (override));
  MOCK_METHOD(void, detach, (EventListenerPtr<PersonEvent>), (override));
  MOCK_METHOD(void, notify, (std::shared_ptr<PersonEvent>), (override));
};

class EventSourceTests : public ::testing::Test {
protected:
  void SetUp() override { table = make_shared<RegistrationTableMock>(); }

  shared_ptr<RegistrationTableMock> table;
};

class ConventionBooth : public EventListener<PersonEvent> {
public:
  ConventionBooth(shared_ptr<RegistrationTableMock> table)
      : EventListener(table) {}

  void handleEvent(std::shared_ptr<PersonEvent>) override {}
};

TEST_F(EventSourceTests, canAttachAndDetatch) {
  EXPECT_CALL(*table, attach(::testing::_)).Times(1);
  EXPECT_CALL(*table, detach(::testing::_)).Times(1);
  EXPECT_NO_THROW(make_shared<ConventionBooth>(table));
}

TEST(EventSourceExceptionTests, canThrowInvalidArgument) {
  auto non_existant_table = shared_ptr<RegistrationTableMock>();

  EXPECT_THROW(make_shared<ConventionBooth>(non_existant_table),
               std::invalid_argument);
}

TEST(EventSourceExceptionTests, canDetachFromDeadBroadcaster) {
  auto non_existant_table = make_shared<RegistrationTableMock>();

  shared_ptr<ConventionBooth> booth;
  EXPECT_NO_THROW(booth = make_shared<ConventionBooth>(non_existant_table));

  non_existant_table.reset();
  EXPECT_NO_THROW(booth.reset());
}