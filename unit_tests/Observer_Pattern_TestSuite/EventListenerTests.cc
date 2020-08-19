#include "ObserverPatterTestsShared.hpp"
#include "Observer_Pattern.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace std;
using namespace ObserverPattern;

class RegistrationTable : public EventBroadcaster<PersonEvent> {
public:
  void registerPerson(string name) {
    notify(make_shared<PersonEvent>(name, PersonEventType::REGISTERED));
  }
  void deregisterPerson(string name) {
    notify(make_shared<PersonEvent>(name, PersonEventType::DEREGISTERED));
  }
};

class ConventionBoothMock : public EventListener<PersonEvent> {
public:
  ConventionBoothMock(shared_ptr<RegistrationTable> table)
      : EventListener(table) {}

  MOCK_METHOD(void, handleEvent, (shared_ptr<PersonEvent>), (override));
};

class EventListenerTests : public ::testing::Test {
protected:
  void SetUp() override {
    table = make_shared<RegistrationTable>();
    booth = make_shared<ConventionBoothMock>(table);
  }

  shared_ptr<RegistrationTable> table;
  shared_ptr<ConventionBoothMock> booth;
};

MATCHER_P(EventPtrIsEqual, value, "") {
  if (arg->getName() == value->getName() &&
      arg->getEventType() == value->getEventType())
    return true;
  else
    return false;
}

TEST_F(EventListenerTests, canNotifyOnRegistration) {
  EXPECT_CALL(*booth, handleEvent(EventPtrIsEqual(make_shared<PersonEvent>(
                          "Bob", PersonEventType::REGISTERED))))
      .Times(1);

  table->registerPerson("Bob");
}

TEST_F(EventListenerTests, canNotifyOnDeRegistration) {
  EXPECT_CALL(*booth, handleEvent(EventPtrIsEqual(make_shared<PersonEvent>(
                          "Bob", PersonEventType::DEREGISTERED))))
      .Times(1);

  table->deregisterPerson("Bob");
}