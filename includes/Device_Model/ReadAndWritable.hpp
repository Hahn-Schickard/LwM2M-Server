#ifndef __LWM2M_READ_AND_WRITABLE_RESOURCE_HPP
#define __LWM2M_READ_AND_WRITABLE_RESOURCE_HPP

#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "Readable.hpp"
#include "Writable.hpp"

namespace LwM2M {
/**
 * @addtogroup Device_Model
 * @{
 */

/**
 * @brief Models the functionality of read and writable resource.
 *
 * This trait is a composition of Readable and Writable, to learn how to use
 * each part:
 *
 * @see Readable
 * @see Writable
 *
 */
class ReadAndWritable : public Readable, public Writable {
public:
  ReadAndWritable(Observable::ExceptionHandler handler,
      ObservableInterfacePtr observe_requester,
      ReadableInterfacePtr read_requester,
      ExecutableInterfacePtr write_requester, EndpointPtr endpoint,
      ElementID id, DataType data_type);
};

using ReadAndWritableSharedPtr = std::shared_ptr<ReadAndWritable>;
using ReadAndWritablePtr =
    NonemptyPointer::NonemptyPtr<ReadAndWritableSharedPtr>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_READ_AND_WRITABLE_RESOURCE_HPP