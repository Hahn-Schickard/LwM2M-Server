#ifndef __GENERIC_OBSERVERR_PATTERN_HPP
#define __GENERIC_OBSERVERR_PATTERN_HPP

#include "stdexcept"
#include <memory>
#include <unordered_set>

namespace ObserverPattern {
template <typename EventType> class EventBroadcasterInterface;
template <typename EventType>
using EventBroadcasterInterfacePtr =
    std::shared_ptr<EventBroadcasterInterface<EventType>>;

template <typename EventType> class EventListener {
  EventBroadcasterInterfacePtr<EventType> broadcaster_;

public:
  EventListener(EventBroadcasterInterfacePtr<EventType> broadcaster)
      : broadcaster_(broadcaster) {
    if (broadcaster)
      broadcaster_->attach(this);
    else
      throw std::invalid_argument(
          "EventBroadcasterInterfacePtr can not be a nullptr.");
  }

  ~EventListener() { broadcaster_->detach(this); }

  virtual void handleEvent(std::shared_ptr<EventType>) = 0;
};

template <typename EventType>
using EventListenerPtr = EventListener<EventType> *;

template <typename EventType> class EventBroadcasterInterface {
public:
  virtual ~EventBroadcasterInterface() = default;

  virtual void attach(EventListenerPtr<EventType> listener) = 0;
  virtual void detach(EventListenerPtr<EventType> listener) = 0;
  virtual void notify(std::shared_ptr<EventType> event) = 0;
};

template <typename EventType>
class EventBroadcaster : public EventBroadcasterInterface<EventType> {
  std::unordered_set<EventListenerPtr<EventType>> listeners_;

public:
  void attach(EventListenerPtr<EventType> listener) override {
    listeners_.insert(listener);
  }

  void detach(EventListenerPtr<EventType> listener) override {
    listeners_.erase(listeners_.find(listener));
  }

  void notify(std::shared_ptr<EventType> event) override {
    for (auto listener : listeners_) {
      listener->handleEvent(event);
    }
  }
};
} // namespace ObserverPattern

#endif //__GENERIC_OBSERVERR_PATTERN_HPP