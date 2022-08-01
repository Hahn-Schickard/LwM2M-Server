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

/**
 * @brief Models a cancelable request result, issued by CancelableInterface
 * inheritors
 *
 * @tparam T - promised result type
 */
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

  /**
   * @brief Waits until the promised result is available and cleanups the
   * cancelable request, once the result has been obtained
   *
   * @return T - promised result type
   */
  T get() {
    auto result = result_.get();
    if (auto cleanupRequest = cleanupRequest_.lock()) {
      (*cleanupRequest)(
          id_); // if cleaner is not available, all request were already cleaned
    }
    return result;
  }

  /**
   * @brief Calls the get() method in a separate thread and returns a future to
   * the promised result
   *
   * @return std::future<T>
   */
  std::future<T> asyncGet() {
    return std::async(std::launch::async, &RequestResult::get, this);
  }

  /**
   * @brief Cancels the requested result.
   *
   * @note The user must call either get() or asyncGet() to handle the promised
   * result (which will be an exception) after the call to cancel() method. If
   * this is not done the destructor of RequestResult will throw an exception
   * when cleaning up the promised result.
   *
   * @throws std::runtime_error if linked CancelableInterface callback is no
   * longer available (this can only happen when linked CancelableInterface was
   * destroyed)
   *
   */
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

/**
 * @brief Models the functionality for cancelable requests. Used by Objects,
 * Object Instances, Resources and Resource Instances.
 *
 * MUST be implemented be the Binding
 *
 */
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

  /**
   * @brief Creates a cancelable request. ONLY used by the Device Model
   * elements. This method SHOULD not be used by the users
   *
   * @tparam T - promised result type
   * @param request - a cancelable LwM2M Message from the Server to the Client
   * @param result_future - desired future result type (used when type
   * conversions are required)
   * @return RequestResult<T> - cancelable request result
   */
  template <typename T>
  RequestResult<T> issueCancelable(
      ServerRequestPtr request, std::future<T>&& result_future) {
    auto id = addRequest(request);
    return RequestResult<T>(id, std::move(result_future), cleaner_, canceler_);
  }

private:
  /**
   * @brief Internal method used to add new cancelable messages
   *
   * @param request
   * @return size_t
   */
  size_t addRequest(ServerRequestPtr request) {
    std::lock_guard<std::mutex> guard(request_mutex_);
    auto id = requests_.size() + 1;
    requests_.emplace(id, request);
    return id;
  }

  /**
   * @brief Internal method to remove handled cancelable messages. Used as
   * RequestResult::RequestCleanerPtr callback
   *
   * @param request_id
   */
  void cleanupHandled(size_t request_id) { requests_.erase(request_id); }

  /**
   * @brief Internal method to cancel a given request result.  Used as
   * RequestResult::RequestCancelerPtr callback
   *
   * @param request_id
   */
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