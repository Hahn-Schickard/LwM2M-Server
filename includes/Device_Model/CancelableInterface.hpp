#ifndef __LWM2M_CANCELABLE_INTERFACE_HPP
#define __LWM2M_CANCELABLE_INTERFACE_HPP

#include "Threadsafe_Containers/UnorderedMap.hpp"

#include "Message.hpp"

#include <functional>
#include <future>
#include <mutex>
#include <stdexcept>

namespace LwM2M {

template <typename T> struct RequestResult {
  using RequestCleaner = std::function<void(size_t)>;
  using RequestCanceler = std::function<void(size_t)>;

  RequestResult(size_t request_id, std::future<T>&& result_future,
      RequestCleaner cleanupRequest_callback,
      RequestCanceler cancelRequest_callback)
      : id_(request_id), result_(std::move(result_future)),
        cleanupRequest_(cleanupRequest_callback),
        cancelRequest_(cancelRequest_callback) {}

  T get() {
    auto result = result_.get();
    cleanupRequest_(id_);
    return result;
  }

  std::future<T> asyncGet() { return std::async(std::launch::async, get()); }

  void cancel() { cancelRequest_(id_); }

private:
  size_t id_;
  std::future<T> result_;
  RequestCleaner cleanupRequest_;
  RequestCanceler cancelRequest_;
};

struct CancelableInterface {
  virtual ~CancelableInterface() = default;

  /**
   * @brief Cancels an issued request. Canceled requests must throw and
   * exception, so issues can clean up the memory of the response future
   *
   * @throws std::runtime_error exception if called base virtual implementation
   *
   */
  virtual void cancelRequest(ServerRequestPtr /*message*/) {
    throw std::runtime_error("Called base cancelRequest implementation.");
  }

  template <typename T>
  RequestResult<T> issueCancelable(
      ServerRequestPtr request, std::future<T>&& result_future) {
    auto id = addRequest(request);
    auto cleanup_cb = std::bind(
        &CancelableInterface::cleanupHandled, this, std::placeholders::_1);
    auto cancel_cb = std::bind(
        &CancelableInterface::cancelIssued, this, std::placeholders::_1);

    return RequestResult<T>(
        id, std::move(result_future), cleanup_cb, cancel_cb);
  }

private:
  size_t addRequest(ServerRequestPtr request) {
    std::lock_guard<std::mutex> guard(request_mutex_);
    auto id = requests_.size() + 1;
    requests_.emplace(id, request);
    return id;
  }

  void cleanupHandled(size_t request_id) { requests_.erase(request_id); }

  void cancelIssued(size_t request_id) {
    auto it = requests_.find(request_id);
    if (it != requests_.end()) {
      cancelRequest(it->second);
      cleanupHandled(request_id);
    }
  }

  Threadsafe::UnorderedMap<size_t, ServerRequestPtr> requests_;
  std::mutex request_mutex_;
};

using CancelableInterfacePtr = std::shared_ptr<CancelableInterface>;
} // namespace LwM2M

#endif //__LWM2M_CANCELABLE_INTERFACE_HPP