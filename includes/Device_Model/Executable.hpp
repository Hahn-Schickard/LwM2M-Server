#ifndef __LWM2M_EXECUTABLE_RESOURCE_HPP
#define __LWM2M_EXECUTABLE_RESOURCE_HPP

#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "CallableEntity.hpp"
#include "ExecutableInterface.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {
class Executable : public CallableEntity {
public:
  Executable(
      ExecutableInterfacePtr requester, EndpointPtr endpoint, ElementID id);

  std::future<bool> execute(std::string arguments);

private:
  ExecutableInterfacePtr requester_;
};

using ExecutablePtr = NonemptyPointer::NonemptyPtr<Executable>;
} // namespace LwM2M
#endif //__LWM2M_EXECUTABLE_RESOURCE_HPP