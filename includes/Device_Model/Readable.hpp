#ifndef __LWM2M_READABLE_RESOURCE_HPP
#define __LWM2M_READABLE_RESOURCE_HPP

#include "Nonempty/Pointer.hpp"

#include "Observable.hpp"
#include "ReadableInterface.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {
/**
 * @addtogroup Device_Model
 * @{
 */

/**
 * @brief Models the functionality of a readable resource
 *
 */
struct Readable : public Observable {
  /**
   * @brief Read request result
   *
   * @see RequestResult for more information
   *
   */
  using Result = RequestResult<DataVariant>;

  Readable(Observable::ExceptionHandler handler,
      ObservableInterfacePtr observe_requester,
      ReadableInterfacePtr read_requester, EndpointPtr endpoint, ElementID id,
      DataType data_type);

  /**
   * @brief creates a read request to the LwM2M Client, the result of which is
   * the DataVariant of the modeled DataType
   *
   * @return Result<DataVariant> - based on modeled resources DataType enum
   * value
   */
  Result read();

private:
  ReadableInterfacePtr requester_;
};

using ReadableSharedPtr = std::shared_ptr<Readable>;
using ReadablePtr = Nonempty::Pointer<ReadableSharedPtr>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_READABLE_RESOURCE_HPP