#ifndef __LWM2M_CANCELABLE_INTERFACE_HPP
#define __LWM2M_CANCELABLE_INTERFACE_HPP

#include "Message.hpp"

#include <future>
#include <stdexcept>

namespace LwM2M {

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
};

using CancelableInterfacePtr = std::shared_ptr<CancelableInterface>;
} // namespace LwM2M

#endif //__LWM2M_CANCELABLE_INTERFACE_HPP