#ifndef __LWM2M_WRITABLE_RESOURCE_HPP
#define __LWM2M_WRITABLE_RESOURCE_HPP

#include "Nonempty_Pointer/NonemptyPtr.hpp"

#include "CallableEntity.hpp"
#include "ExecutableInterface.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {

class Writable : public CallableEntity {
public:
  Writable(
      ExecutableInterfacePtr requester, EndpointPtr endpoint, ElementID id);

  std::future<bool> write(DataVariant data);

private:
  ExecutableInterfacePtr requester_;
};

using WritableSharedPtr = std::shared_ptr<Writable>;
using WritablePtr = NonemptyPointer::NonemptyPtr<WritableSharedPtr>;

} // namespace LwM2M

#endif //__LWM2M_WRITABLE_RESOURCE_HPP