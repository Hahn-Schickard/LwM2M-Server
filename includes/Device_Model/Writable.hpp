#ifndef __LWM2M_WRITABLE_RESOURCE_HPP
#define __LWM2M_WRITABLE_RESOURCE_HPP

#include "ElementAddress.hpp"
#include "ExecutableInterface.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {

class Writable : public ElementAddress {
public:
  Writable(
      ExecutableInterfacePtr requester, EndpointPtr endpoint, ElementID id);

  std::future<bool> write(DataVariant data);

private:
  ExecutableInterfacePtr requester_;
};

using WritablePtr = std::shared_ptr<Writable>;

} // namespace LwM2M

#endif //__LWM2M_WRITABLE_RESOURCE_HPP