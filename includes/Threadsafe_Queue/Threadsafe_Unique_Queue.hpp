#ifndef _THREAD_SAFE_UNIQUE_QUEUE_HPP
#define _THREAD_SAFE_UNIQUE_QUEUE_HPP

#include <condition_variable>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <unordered_set>

template <typename T> class ThreadsafeUniqueQueue {
  struct Node {
    std::shared_ptr<T> data;
    std::unique_ptr<Node> next;
  };

  struct WeakPtrHash {
  public:
    size_t operator()(std::weak_ptr<T> weak_ptr) const {
      auto ptr = weak_ptr.lock();
      return std::hash<T>()(*ptr);
    }
  };

  struct WeakPtrEqual {
  public:
    bool operator()(std::weak_ptr<T> weak_ptr1,
                    std::weak_ptr<T> weak_ptr2) const {
      auto ptr1 = weak_ptr1.lock();
      auto ptr2 = weak_ptr2.lock();
      return std::hash<T>()(*ptr1) == std::hash<T>()(*ptr2) ? true : false;
    }
  };

  std::mutex head_mutex;
  std::unique_ptr<Node> head;
  std::mutex tail_mutex;
  Node *tail;
  std::condition_variable data_cond;
  std::unordered_set<std::weak_ptr<T>, WeakPtrHash, WeakPtrEqual> lookup;

  Node *get_tail() {
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    return tail;
  }

  std::unique_ptr<Node> pop_head() {
    std::unique_ptr<Node> old_head = std::move(head);
    head = std::move(old_head->next);
    lookup.erase(lookup.find(old_head->data));
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
    value = *head->data;
    return pop_head();
  }

public:
  ThreadsafeUniqueQueue() : head(new Node), tail(head.get()) {}
  ThreadsafeUniqueQueue(const ThreadsafeUniqueQueue &other) = delete;
  ThreadsafeUniqueQueue &operator=(const ThreadsafeUniqueQueue &other) = delete;

  /**
   * @brief Checks if the queue has some data to return
   *
   * @return std::unique_ptr<T>
   */
  std::unique_ptr<T> try_pop() {
    std::unique_ptr<Node> old_head = try_pop_head();
    return old_head ? std::make_unique<T>(*old_head->data.get())
                    : std::unique_ptr<T>();
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
    return std::make_unique<T>(*old_head->data.get());
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
   * @brief Saves a given value within the queue if no duplicate exists.
   *
   * When pushing a custom object, you need to provide a specialization for
   * size_t hash<Key>::operator() as specifed by:
   * https://en.cppreference.com/w/cpp/utility/hash/operator()
   *
   * A simple example of such implementation could be:
   * @code {.cpp}
    struct Custom {
      string name;
      int age;
    };
    namespace std {
    template <> struct hash<Custom> {
    public:
      size_t operator()(const Custom &value) const {
        return hash<string>{}(value.name) + hash<int>{}(value.age);
      }
    };
    } // namespace std
   * @endcode
   *
   * @param new_value
   */
  void push(T new_value) {
    auto value = std::make_shared<T>(new_value);
    if (lookup.find(value) == lookup.end()) {
      std::unique_ptr<Node> p(new Node);
      {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = move(value);
        lookup.insert(tail->data);
        Node *const new_tail = p.get();
        tail->next = std::move(p);
        tail = new_tail;
      }
      data_cond.notify_one();
    }
  }

  /**
   * @brief Saves the value of a std::unique_ptr by calling
   * std::unique_ptr::get() if there are no duplicates witih the queue,
   * otherwise discards the std::unique_ptr
   *
   * @param new_value
   */
  void push(std::unique_ptr<T> new_value) { push(*new_value.get()); }
};

#endif //_THREAD_SAFE_UNIQUE_QUEUE_HPP