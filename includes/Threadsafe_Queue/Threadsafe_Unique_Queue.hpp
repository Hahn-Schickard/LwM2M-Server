#ifndef _THREAD_SAFE_UNIQUE_QUEUE_HPP
#define _THREAD_SAFE_UNIQUE_QUEUE_HPP

#include "Threadsafe_Queue.hpp"
#include <functional>
#include <unordered_set>

template <typename T> struct node {
  std::unique_ptr<T> data;
  std::unique_ptr<node> next;

  bool operator==(const node &object) {
    return std::hash<T>{}(data.get()) == std::hash<T>{}(object.data.get());
  }
};

namespace std {
template <typename T> struct hash<node<T>> {
public:
  size_t operator()(const node<T> &value) const {
    return hash<T>{}(value.data.get());
  }
};
} // namespace std

template <typename T> class ThreadsafeUniqueQueue {
  typedef typename std::reference_wrapper<const node<T>> node_reference;

  std::mutex head_mutex;
  std::unique_ptr<node<T>> head;
  std::mutex tail_mutex;
  node<T> *tail;
  std::condition_variable data_cond;
  std::unordered_set<node_reference> lookup_table_;

  node<T> *get_tail() {
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    return tail;
  }

  std::unique_ptr<node<T>> pop_head() {
    std::unique_ptr<node<T>> old_head = std::move(head);
    head = std::move(old_head->next);
    lookup_table_.erase(lookup_table_.find(*old_head.get()));
    return old_head;
  }

  std::unique_lock<std::mutex> wait_for_data() {
    std::unique_lock<std::mutex> head_lock(head_mutex);
    data_cond.wait(head_lock, [&] { return head.get() != get_tail(); });
    return std::move(head_lock);
  }

  std::unique_ptr<node<T>> wait_pop_head() {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    return pop_head();
  }

  std::unique_ptr<node<T>> wait_pop_head(T &value) {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    value = std::move(*head->data);
    return pop_head();
  }

  std::unique_ptr<node<T>> try_pop_head() {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if (head.get() == get_tail()) {
      return std::unique_ptr<node<T>>();
    }
    return pop_head();
  }

  std::unique_ptr<node<T>> try_pop_head(T &value) {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if (head.get() == get_tail()) {
      return std::unique_ptr<node<T>>();
    }
    value = std::move(*head->data);
    return pop_head();
  }

public:
  ThreadsafeUniqueQueue()
      : head(new node<T>), tail(head.get()),
        lookup_table_(std::unordered_set<node_reference>()) {}
  ThreadsafeUniqueQueue(const ThreadsafeUniqueQueue &other) = delete;
  ThreadsafeUniqueQueue &operator=(const ThreadsafeUniqueQueue &other) = delete;

  std::unique_ptr<T> try_pop() {
    std::unique_ptr<node<T>> old_head = try_pop_head();
    return old_head ? move(old_head->data) : std::unique_ptr<T>();
  }

  bool try_pop(T &value) {
    std::unique_ptr<node<T>> const old_head = try_pop_head(value);
    return old_head ? true : false;
  }

  bool empty() {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    return (head.get() == get_tail());
  }

  std::unique_ptr<T> wait_and_pop() {
    std::unique_ptr<node<T>> const old_head = wait_pop_head();
    return move(old_head->data);
  }

  void wait_and_pop(T &value) {
    std::unique_ptr<node<T>> const old_head = wait_pop_head(value);
  }

  void push(T new_value) { push(std::make_unique<T>(std::move(new_value))); }

  void push(std::unique_ptr<T> new_value) {
    if (new_value && lookup_table_.find(std::cref(*new_value.get())) ==
                         lookup_table_.end()) {
      std::unique_ptr<node<T>> p(new node<T>);
      {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = move(new_value);
        node<T> *const new_tail = p.get();
        tail->next = std::move(p);
        tail = new_tail;
        lookup_table_.insert(*tail);
      }
      data_cond.notify_one();
    }
  }
};

#endif //_THREAD_SAFE_UNIQUE_QUEUE_HPP