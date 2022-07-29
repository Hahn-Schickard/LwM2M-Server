#ifndef __LWM2M_WRITABLE_RESOURCE_HPP
#define __LWM2M_WRITABLE_RESOURCE_HPP

#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "ElementAddress.hpp"
#include "ExecutableInterface.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {

struct Writable : public ElementAddress {
  using Result = RequestResult<bool>;

  Writable(
      ExecutableInterfacePtr requester, EndpointPtr endpoint, ElementID id);

  Result write(DataVariant data);

private:
  ExecutableInterfacePtr requester_;
};

using WritableSharedPtr = std::shared_ptr<Writable>;
using WritablePtr = NonemptyPointer::NonemptyPtr<WritableSharedPtr>;

} // namespace LwM2M

#endif //__LWM2M_WRITABLE_RESOURCE_HPP