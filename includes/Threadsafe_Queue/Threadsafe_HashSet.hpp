#ifndef __THREADSAFE_HASH_SET_HPP
#define __THREADSAFE_HASH_SET_HPP

#include <algorithm>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

template <typename T> class ThreadsafeHashSet {
  std::unordered_map<size_t, std::shared_ptr<T>> entries_;
  std::vector<std::shared_ptr<T>> entry_queue_;

  std::mutex entry_mutex_;

  void insert_value(size_t index, std::shared_ptr<T> entry_value) {
    std::lock_guard<std::mutex> lock(entry_mutex_);
    auto entry = std::make_pair(index, entry_value);
    entries_.insert(entry);
    entry_queue_.push_back(entry_value);
  }

  bool exists(size_t hash) { return entries_.find(hash) != entries_.end(); }

  size_t getHash(T value) { return std::hash<T>{}(value); }

public:
  ThreadsafeHashSet()
      : entries_(std::unordered_map<size_t, std::shared_ptr<T>>()),
        entry_queue_(std::vector<std::shared_ptr<T>>()) {}
  ThreadsafeHashSet(const ThreadsafeHashSet &other) = delete;
  ThreadsafeHashSet &operator=(const ThreadsafeHashSet &other) = delete;

  size_t size() { return entries_.size(); }

  bool empty() { return entries_.empty(); }

  void push(std::shared_ptr<T> value) {
    T entry_value = *value.get();
    size_t index = getHash(entry_value);
    if (!exists(index)) {
      insert_value(index, value);
    }
  }

  void push(T value) { push(std::make_shared<T>(value)); }

  std::shared_ptr<T> front() {
    if (entry_queue_.empty()) {
      return std::shared_ptr<T>();
    } else {
      std::lock_guard<std::mutex> lock(entry_mutex_);
      return entry_queue_.front();
    }
  }

  void front(std::shared_ptr<T> &buffer) { buffer = front(); }

  void pop(std::shared_ptr<T> entry) {
    T value = *entry.get();
    size_t index = getHash(value);
    if (exists(index)) {
      std::lock_guard<std::mutex> lock(entry_mutex_);
      entries_.erase(index);
      entry_queue_.erase(
          std::remove(entry_queue_.begin(), entry_queue_.end(), entry),
          entry_queue_.end());
    }
  }
};

#endif //__THREADSAFE_HASH_SET_HPP
