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

class RegistrationTableMock : public EventBroadcaster<PersonEvent> {
  using EventListenerPtr = EventListener<PersonEvent> *;

public:
  MOCK_METHOD(void, attach, (EventListenerPtr), (override));
  MOCK_METHOD(void, detach, (EventListenerPtr), (override));
  MOCK_METHOD(void, notify, (std::shared_ptr<PersonEvent>), (override));
};

class ConventionBooth : public EventListener<PersonEvent> {
public:
  ConventionBooth(shared_ptr<RegistrationTableMock> table)
      : EventListener(table) {}

  void handleEvent(std::shared_ptr<PersonEvent> event) override {}
};

TEST(EventBroadcasterTests, canAttach) {
  auto table = make_shared<RegistrationTableMock>();
  shared_ptr<ConventionBooth> booth;

  EXPECT_CALL(*table, attach(booth.get())).Times(1);
  booth = make_shared<ConventionBooth>(table);
}