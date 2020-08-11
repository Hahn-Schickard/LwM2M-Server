#ifndef __THREADSAFE_QUEUE_INTERFACE_HPP
#define __THREADSAFE_QUEUE_INTERFACE_HPP

#include <memory>
#include <stdexcept>

struct NotImplemented : public std::logic_error {
  NotImplemented() : std::logic_error("Method has no specialization!") {}
};

template <typename T> class QueueInterface {
public:
  std::unique_ptr<T> try_pop() { throw NotImplemented(); }
  bool try_pop(T &value) { throw NotImplemented(); }
  bool empty() { throw NotImplemented(); }
  std::unique_ptr<T> wait_and_pop() { throw NotImplemented(); }
  void wait_and_pop(T &value) { throw NotImplemented(); }
  void push(T new_value) { throw NotImplemented(); }
  void push(std::unique_ptr<T> new_value) { throw NotImplemented(); }
};

#endif //__THREADSAFE_QUEUE_INTERFACE_HPP