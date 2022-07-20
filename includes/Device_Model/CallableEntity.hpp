#ifndef __LWM2M_CALLABLE_ENTITY_HPP
#define __LWM2M_CALLABLE_ENTITY_HPP

#include "ElementID.hpp"
#include "Endpoint.hpp"

namespace LwM2M {

struct CallableEntity {
  CallableEntity(EndpointPtr endpoint, ElementID id)
      : endpoint_(endpoint), id_(id) {}

  EndpointPtr getEndpoint() { return endpoint_; }

  ElementID getID() { return id_; }

protected:
  EndpointPtr endpoint_;
  ElementID id_;
};

using CallableEntityPtr = std::shared_ptr<CallableEntity>();
} // namespace LwM2M

#endif //__LWM2M_CALLABLE_ENTITY_HPP