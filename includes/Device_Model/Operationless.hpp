#ifndef __LWM2M_OPERATIONLESS_RESOURCE_HPP
#define __LWM2M_OPERATIONLESS_RESOURCE_HPP

#include "ResourceDescriptor.hpp"

namespace LwM2M {

class Operationless {
public:
  Operationless() = default;
};

using OperationlessPtr = std::shared_ptr<Operationless>;
} // namespace LwM2M

#endif //__LWM2M_OPERATIONLESS_RESOURCE_HPP