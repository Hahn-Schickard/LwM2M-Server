#ifndef __LWM2M_CANCELABLE_INTERFACE_HPP
#define __LWM2M_CANCELABLE_INTERFACE_HPP

#include "Message.hpp"

#include <future>
#include <stdexcept>

namespace LwM2M {

struct CancelableInterface {
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
};

using CancelableInterfacePtr = std::shared_ptr<CancelableInterface>;
} // namespace LwM2M

#endif //__LWM2M_CANCELABLE_INTERFACE_HPP