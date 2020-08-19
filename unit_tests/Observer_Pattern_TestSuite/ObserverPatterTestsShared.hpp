#ifndef __SHARED_RESOURCE_DECLARATIONS_FOR_OBSERVER_PATTERN_TESTS_HPP
#define __SHARED_RESOURCE_DECLARATIONS_FOR_OBSERVER_PATTERN_TESTS_HPP

#include <string>

enum class PersonEventType { REGISTERED, DEREGISTERED };

class PersonEvent {
  std::string name;
  PersonEventType event;

public:
  PersonEvent(std::string person_name, PersonEventType event_type)
      : name(move(person_name)), event(event_type) {}

  bool operator==(const PersonEvent &comparison) {
    if (name == comparison.name && event == comparison.event)
      return true;
    else
      return false;
  }

  const std::string &getName() const { return name; }
  PersonEventType getEventType() const { return event; }
};

#endif //__SHARED_RESOURCE_DECLARATIONS_FOR_OBSERVER_PATTERN_TESTS_HPP