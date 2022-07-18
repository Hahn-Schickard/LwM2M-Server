#ifndef __LWM2M_EXECUTABLE_RESOURCE_HPP
#define __LWM2M_EXECUTABLE_RESOURCE_HPP

#include "CallableEntity.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {
class Executable : public CallableEntity {
public:
  Executable(RequesterPtr requester, EndpointPtr endpoint, ElementID id);

  std::future<bool> execute(std::string arguments);
};

using ExecutablePtr = std::shared_ptr<Executable>;
} // namespace LwM2M
#endif //__LWM2M_EXECUTABLE_RESOURCE_HPP