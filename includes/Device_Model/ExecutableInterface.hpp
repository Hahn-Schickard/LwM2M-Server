#ifndef __LWM2M_EXECUTABLE_INTERFACE_HPP
#define __LWM2M_EXECUTABLE_INTERFACE_HPP

#include "CancelableInterface.hpp"
#include "DeviceManagmentMessage.hpp"

#include <future>
#include <stdexcept>

namespace LwM2M {

/**
 * @brief Models the functionality for executable requests. Used by Executable
 * resource traits only
 *
 * MUST be implemented be the Binding
 *
 */
struct ExecutableInterface : public CancelableInterface {
  virtual ~ExecutableInterface() = default;

  /**
   * @brief Request LwM2M::Device to do an Action
   *
   * @throws LwM2M::ResponseReturnedAnErrorCode if the corresponding
   * LwM2M::Response contained an error code
   * @throws std::logic_error if an internal failure occurred
   * @throws std::runtime_error exception if called base virtual implementation
   *
   * @param message
   * @return std::future<bool>
   */
  virtual std::future<bool> requestAction(
      const DeviceManagementRequestPtr& /*message*/) {
    throw std::runtime_error("Called base requestAction implementation.");
  }
};

using ExecutableInterfacePtr = std::shared_ptr<ExecutableInterface>;
} // namespace LwM2M

#endif //__LWM2M_EXECUTABLE_INTERFACE_HPP