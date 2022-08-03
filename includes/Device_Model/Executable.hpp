#ifndef __LWM2M_EXECUTABLE_RESOURCE_HPP
#define __LWM2M_EXECUTABLE_RESOURCE_HPP

#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "ElementAddress.hpp"
#include "ExecutableInterface.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {
/**
 * @addtogroup Device_Model
 * @{
 */

/**
 * @brief Models the functionality of executable resource
 *
 */
struct Executable : public ElementAddress {
  /**
   * @brief Execute request result
   *
   * @see RequestResult for more information
   *
   */
  using Result = RequestResult<bool>;

  Executable(
      ExecutableInterfacePtr requester, EndpointPtr endpoint, ElementID id);

  /**
   * @brief creates an execute request to the LwM2M Client with a given
   * std::string value, which contains the arguments of the issued command
   *
   * @param arguments
   * @return Result<true> if request was successful
   */
  Result execute(std::string arguments);

private:
  ExecutableInterfacePtr requester_;
};

using ExecutableSharedPtr = std::shared_ptr<Executable>;
using ExecutablePtr = NonemptyPointer::NonemptyPtr<ExecutableSharedPtr>;

/** @}*/
} // namespace LwM2M
#endif //__LWM2M_EXECUTABLE_RESOURCE_HPP