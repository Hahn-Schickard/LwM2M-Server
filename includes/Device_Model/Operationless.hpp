#ifndef __LWM2M_OPERATIONLESS_RESOURCE_HPP
#define __LWM2M_OPERATIONLESS_RESOURCE_HPP

#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "ResourceDescriptor.hpp"

namespace LwM2M {

class Operationless {
public:
  Operationless() = default;
};

using OperationlessSharedPtr = std::shared_ptr<Operationless>;
using OperationlessPtr = NonemptyPointer::NonemptyPtr<OperationlessSharedPtr>;
} // namespace LwM2M

#endif //__LWM2M_OPERATIONLESS_RESOURCE_HPP