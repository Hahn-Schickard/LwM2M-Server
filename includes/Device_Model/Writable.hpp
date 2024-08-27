#ifndef __LWM2M_WRITABLE_RESOURCE_HPP
#define __LWM2M_WRITABLE_RESOURCE_HPP

#include "Nonempty/Pointer.hpp"

#include "ElementAddress.hpp"
#include "ExecutableInterface.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {
/**
 * @addtogroup Device_Model
 * @{
 */

/**
 * @brief Model a resource with write-only access
 *
 */
struct Writable : public ElementAddress {
  /**
   * @brief Write request result
   *
   * @see RequestResult for more information
   *
   */
  using Result = RequestResult<bool>;

  Writable(const ExecutableInterfacePtr& requester, const EndpointPtr& endpoint,
      const ElementID& id);

  /**
   * @brief creates a write request to the LwM2M Client with a given DataVariant
   * value
   *
   * @param data
   * @return Result<true> if request was successful
   */
  Result write(const DataVariant& data);

private:
  ExecutableInterfacePtr requester_;
};

using WritableSharedPtr = std::shared_ptr<Writable>;
using WritablePtr = Nonempty::Pointer<WritableSharedPtr>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_WRITABLE_RESOURCE_HPP