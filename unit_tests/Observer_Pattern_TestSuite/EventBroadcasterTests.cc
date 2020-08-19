#include "ObserverPatterTestsShared.hpp"
#include "Observer_Pattern.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

using namespace std;
using namespace ObserverPattern;

class RegistrationTableMock : public EventBroadcasterInterface<PersonEvent> {
  using EventListenerPtr = EventListener<PersonEvent> *;

public:
  MOCK_METHOD(void, attach, (EventListenerPtr), (override));
  MOCK_METHOD(void, detach, (EventListenerPtr), (override));
  MOCK_METHOD(void, notify, (std::shared_ptr<PersonEvent>), (override));
};

class EventBroadcasterTests : public ::testing::Test {
protected:
  void SetUp() override { table = make_shared<RegistrationTableMock>(); }

  shared_ptr<RegistrationTableMock> table;
};

class ConventionBooth : public EventListener<PersonEvent> {
public:
  ConventionBooth(shared_ptr<RegistrationTableMock> table)
      : EventListener(table) {}

  void handleEvent(std::shared_ptr<PersonEvent> event) override {}
};

TEST_F(EventBroadcasterTests, canAttach) {
  EXPECT_CALL(*table, attach(::testing::_)).Times(1);
  EXPECT_NO_THROW(make_shared<ConventionBooth>(table));
}