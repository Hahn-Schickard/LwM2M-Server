#ifndef __LWM2M_WRITABLE_RESOURCE_HPP
#define __LWM2M_WRITABLE_RESOURCE_HPP

#include "CallableEntity.hpp"
#include "ResourceDescriptor.hpp"

namespace LwM2M {

class Writable : public CallableEntity {
public:
  Writable(RequesterInterfaceFacadePtr requester, EndpointPtr endpoint,
      ElementID id);

  std::future<bool> write(DataVariant data);
};

using WritablePtr = std::shared_ptr<Writable>;

} // namespace LwM2M

#endif //__LWM2M_WRITABLE_RESOURCE_HPP