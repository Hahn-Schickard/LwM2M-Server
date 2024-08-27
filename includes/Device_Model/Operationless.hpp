#ifndef __LWM2M_OPERATIONLESS_RESOURCE_HPP
#define __LWM2M_OPERATIONLESS_RESOURCE_HPP

#include "Nonempty/Pointer.hpp"

#include "ResourceDescriptor.hpp"

namespace LwM2M {
/**
 * @addtogroup Device_Model
 * @{
 */

/**
 * @brief Models the functionality of operationless resource (A resource that
 * only has static information defined in the descriptor)
 *
 */
class Operationless {
public:
  Operationless() = default;
};

using OperationlessSharedPtr = std::shared_ptr<Operationless>;
using OperationlessPtr = Nonempty::Pointer<OperationlessSharedPtr>;

/** @}*/
} // namespace LwM2M

#endif //__LWM2M_OPERATIONLESS_RESOURCE_HPP