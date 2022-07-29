#ifndef __LWM2M_CANCELABLE_INTERFACE_HPP
#define __LWM2M_CANCELABLE_INTERFACE_HPP

#include "Threadsafe_Containers/SharedPtr.hpp"
#include "Threadsafe_Containers/UnorderedMap.hpp"

#include "Message.hpp"

#include <functional>
#include <future>
#include <mutex>
#include <stdexcept>

namespace LwM2M {

template <typename T> struct RequestResult {
  using Functor = std::function<void(size_t)>;
  using RequestCleanerPtr = Threadsafe::WeakPtr<Functor>;
  using RequestCancelerPtr = Threadsafe::WeakPtr<Functor>;

  RequestResult(size_t request_id, std::future<T>&& result_future,
      RequestCleanerPtr cleanupRequest_callback,
      RequestCancelerPtr cancelRequest_callback)
      : id_(request_id), result_(std::move(result_future)),
        cleanupRequest_(cleanupRequest_callback),
        cancelRequest_(cancelRequest_callback) {}

  T get() {
    auto result = result_.get();
    if (auto cleanupRequest = cleanupRequest_.lock()) {
      (*cleanupRequest)(
          id_); // if cleaner is not available, all request were already cleaned
    }
    return result;
  }

  std::future<T> asyncGet() {
    return std::async(std::launch::async, &RequestResult::get, this);
  }

  void cancel() {
    if (auto cancelRequest = cancelRequest_.lock()) {
      (*cancelRequest)(id_);
    } else {
      throw std::runtime_error("RequesterCanceler is not available");
    }
  }

private:
  size_t id_;
  std::future<T> result_;
  RequestCleanerPtr cleanupRequest_;
  RequestCancelerPtr cancelRequest_;
};

struct CancelableInterface {
  CancelableInterface()
      : cleaner_(ThreadsafeFunctor::make(
            std::bind(&CancelableInterface::cleanupHandled, this,
                std::placeholders::_1))),
        canceler_(ThreadsafeFunctor::make(std::bind(
            &CancelableInterface::cancelIssued, this, std::placeholders::_1))) {
  }

  virtual ~CancelableInterface() {
    // @TODO: use blocking destructors for functors to wait until all
    // RequestResult instances are finished with the functors, see issue #132
    // for more information
  }

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
    return RequestResult<T>(id, std::move(result_future), cleaner_, canceler_);
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

  using Functor = std::function<void(size_t)>;
  using ThreadsafeFunctor = Threadsafe::SharedPtr<Functor>;

  ThreadsafeFunctor cleaner_;
  ThreadsafeFunctor canceler_;
  Threadsafe::UnorderedMap<size_t, ServerRequestPtr> requests_;
  std::mutex request_mutex_;
};

using CancelableInterfacePtr = std::shared_ptr<CancelableInterface>;
} // namespace LwM2M

#endif //__LWM2M_CANCELABLE_INTERFACE_HPP