#include "Observer_Pattern.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>
#include <string>

using namespace std;
using namespace ObserverPattern;

enum class PersonEventType { REGISTERED, DEREGISTERED };

class PersonEvent {
  string name;
  PersonEventType event;

public:
  PersonEvent(string person_name, PersonEventType event_type)
      : name(move(person_name)), event(event_type) {}

  bool operator==(const PersonEvent &comparison) {
    if (name == comparison.name && event == comparison.event)
      return true;
    else
      return false;
  }

  const string &getName() const { return name; }
  PersonEventType getEventType() const { return event; }
};

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

MATCHER_P(EventPtrIsEqual, value, "") {
  if (arg->getName() == value->getName() &&
      arg->getEventType() == value->getEventType())
    return true;
  else
    return false;
}

TEST(EventListenerTests, canNotifyOnRegistration) {
  auto table = make_shared<RegistrationTable>();
  auto booth = make_shared<ConventionBoothMock>(table);
  auto expected = make_shared<PersonEvent>("Bob", PersonEventType::REGISTERED);

  EXPECT_CALL(*booth, handleEvent(EventPtrIsEqual(expected))).Times(1);

  table->registerPerson("Bob");
}

TEST(EventListenerTests, canNotifyOnDeRegistration) {
  auto table = make_shared<RegistrationTable>();
  auto booth = make_shared<ConventionBoothMock>(table);

  EXPECT_CALL(*booth, handleEvent(EventPtrIsEqual(make_shared<PersonEvent>(
                          "Bob", PersonEventType::REGISTERED))))
      .Times(1);
  EXPECT_CALL(*booth, handleEvent(EventPtrIsEqual(make_shared<PersonEvent>(
                          "Bob", PersonEventType::DEREGISTERED))))
      .Times(1);

  table->registerPerson("Bob");
  table->deregisterPerson("Bob");
}