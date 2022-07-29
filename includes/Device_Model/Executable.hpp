#ifndef __LWM2M_EXECUTABLE_RESOURCE_HPP
#define __LWM2M_EXECUTABLE_RESOURCE_HPP

#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "ElementAddress.hpp"
#include "ExecutableInterface.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {
struct Executable : public ElementAddress {
  using Result = RequestResult<bool>;

  Executable(
      ExecutableInterfacePtr requester, EndpointPtr endpoint, ElementID id);

  Result execute(std::string arguments);

private:
  ExecutableInterfacePtr requester_;
};

using ExecutableSharedPtr = std::shared_ptr<Executable>;
using ExecutablePtr = NonemptyPointer::NonemptyPtr<ExecutableSharedPtr>;
} // namespace LwM2M
#endif //__LWM2M_EXECUTABLE_RESOURCE_HPP