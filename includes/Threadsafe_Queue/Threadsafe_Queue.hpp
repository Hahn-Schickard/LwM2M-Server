#ifndef _THREAD_SAFE_QUEUE_HPP
#define _THREAD_SAFE_QUEUE_HPP

#include <condition_variable>
#include <memory>
#include <mutex>

/**
 * @brief Thread Safe Queue
 * Based on a Code listings from:
 * A. Williams C++ Concurrency in Action Second Edition, New York, Shelter
 * Island: Manning 2019
 *
 * @tparam T
 */
template <typename T> class ThreadsafeQueue {
private:
  struct Node {
    std::unique_ptr<T> data;
    std::unique_ptr<Node> next;
  };

  std::mutex head_mutex;
  std::unique_ptr<Node> head;
  std::mutex tail_mutex;
  Node *tail;
  std::condition_variable data_cond;

  Node *get_tail() {
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    return tail;
  }

  std::unique_ptr<Node> pop_head() {
    std::unique_ptr<Node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
  }

  std::unique_lock<std::mutex> wait_for_data() {
    std::unique_lock<std::mutex> head_lock(head_mutex);
    data_cond.wait(head_lock, [&] { return head.get() != get_tail(); });
    return std::move(head_lock);
  }

  std::unique_ptr<Node> wait_pop_head() {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    return pop_head();
  }

  std::unique_ptr<Node> wait_pop_head(T &value) {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    value = std::move(*head->data);
    return pop_head();
  }

  std::unique_ptr<Node> try_pop_head() {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if (head.get() == get_tail()) {
      return std::unique_ptr<Node>();
    }
    return pop_head();
  }

  std::unique_ptr<Node> try_pop_head(T &value) {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if (head.get() == get_tail()) {
      return std::unique_ptr<Node>();
    }
    value = std::move(*head->data);
    return pop_head();
  }

public:
  ThreadsafeQueue() : head(new Node), tail(head.get()) {}

  ThreadsafeQueue(const ThreadsafeQueue &other) = delete;
  ThreadsafeQueue &operator=(const ThreadsafeQueue &other) = delete;

  /**
   * @brief Checks if the queue has some data to return
   *
   * @return std::unique_ptr<T>
   */
  std::unique_ptr<T> try_pop() {
    std::unique_ptr<Node> old_head = try_pop_head();
    return old_head ? move(old_head->data) : std::unique_ptr<T>();
  }

  /**
   * @brief Checks if the queue has data to return, if so, it will populate a
   * given buffer, otherwise it will not modify it
   *
   * @param value - buffer that will be populated
   * @return true - populated buffer with data
   * @return false - does not change the buffer
   */
  bool try_pop(T &value) {
    std::unique_ptr<Node> const old_head = try_pop_head(value);
    return old_head ? true : false;
  }

  bool empty() {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    return (head.get() == get_tail());
  }

  /**
   * @brief Blocks the caller until the queue has some data to return
   *
   * @param value
   */
  std::unique_ptr<T> wait_and_pop() {
    std::unique_ptr<Node> const old_head = wait_pop_head();
    return move(old_head->data);
  }

  /**
   * @brief Blocks the caller until the queue has some data to populate a given
   * buffer
   *
   * @param value - buffer that will be populated
   */
  void wait_and_pop(T &value) {
    std::unique_ptr<Node> const old_head = wait_pop_head(value);
  }

  /**
   * @brief Creates a std::unique_ptr from a given value and savs it in a queue
   *
   * @param new_value
   */
  void push(T new_value) { push(std::make_unique<T>(std::move(new_value))); }

  /**
   * @brief Saves a given std::unique_ptr in a queue
   *
   * @param new_value
   */
  void push(std::unique_ptr<T> new_value) {
    if (new_value) {
      std::unique_ptr<Node> p(new Node);
      {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = move(new_value);
        Node *const new_tail = p.get();
        tail->next = std::move(p);
        tail = new_tail;
      }
      data_cond.notify_one();
    }
  }
};

#endif //_THREAD_SAFE_QUEUE_HPP