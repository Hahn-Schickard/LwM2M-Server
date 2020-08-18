#ifndef __GENERIC_OBSERVERR_PATTERN_HPP
#define __GENERIC_OBSERVERR_PATTERN_HPP

#include <memory>
#include <unordered_set>

namespace ObserverPattern {
template <typename EventType> class EventBroadcaster;

template <typename EventType> class EventListener {
  using EventBroadcasterPtr = std::shared_ptr<EventBroadcaster<EventType>>;
  EventBroadcasterPtr broadcaster_;

public:
  EventListener(EventBroadcasterPtr broadcaster) : broadcaster_(broadcaster) {
    broadcaster_->attach(std::shared_ptr<EventListener>(this));
  }

  ~EventListener() {
    broadcaster_->detach(std::shared_ptr<EventListener>(this));
  }

  void handleEvent(EventType);
};

template <typename EventType> class EventBroadcaster {
  using EventListenerPtr = std::shared_ptr<EventListener<EventType>>;
  std::unordered_set<EventListenerPtr> listeners_;

public:
  void attach(EventListenerPtr listener) { listeners_.insert(listener); }

  void detach(EventListenerPtr listener) {
    listeners_.erase(listeners_.find(listener));
  }

  void notify(EventType event) {
    for (auto listener : listeners_) {
      listener->handleEvent(event);
    }
  }
};
} // namespace ObserverPattern

#endif //__GENERIC_OBSERVERR_PATTERN_HPP