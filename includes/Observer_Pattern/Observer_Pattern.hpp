#ifndef __GENERIC_OBSERVERR_PATTERN_HPP
#define __GENERIC_OBSERVERR_PATTERN_HPP

#include "stdexcept"
#include <memory>
#include <unordered_set>

namespace ObserverPattern {
template <typename EventType> class EventBroadcaster;

template <typename EventType> class EventListener {
  using EventBroadcasterPtr = std::shared_ptr<EventBroadcaster<EventType>>;
  EventBroadcasterPtr broadcaster_;

public:
  EventListener(EventBroadcasterPtr broadcaster) : broadcaster_(broadcaster) {
    if (broadcaster)
      broadcaster_->attach(this);
    else
      throw std::invalid_argument("EventBroadcasterPtr can not be a null ptr.");
  }

  ~EventListener() { broadcaster_->detach(this); }

  virtual void handleEvent(std::shared_ptr<EventType>) = 0;
};

template <typename EventType> class EventBroadcasterInterface {
  using EventListenerPtr = EventListener<EventType> *;

public:
  virtual ~EventBroadcasterInterface() = default;

  virtual void attach(EventListenerPtr listener) = 0;
  virtual void detach(EventListenerPtr listener) = 0;
  virtual void notify(std::shared_ptr<EventType> event) = 0;
};

template <typename EventType>
class EventBroadcaster : public EventBroadcasterInterface<EventType> {
  using EventListenerPtr = EventListener<EventType> *;
  std::unordered_set<EventListenerPtr> listeners_;

public:
  void attach(EventListenerPtr listener) override {
    listeners_.insert(listener);
  }

  void detach(EventListenerPtr listener) override {
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