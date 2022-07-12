#ifndef __LWM2M_VALUELESS_RESOURCE_HPP
#define __LWM2M_VALUELESS_RESOURCE_HPP

#include "ResourceDescriptor.hpp"

namespace LwM2M {

class Valueless {
public:
  Valueless() = default;
};

using ValuelessPtr = std::shared_ptr<Valueless>;
} // namespace LwM2M

#endif //__LWM2M_VALUELESS_RESOURCE_HPP