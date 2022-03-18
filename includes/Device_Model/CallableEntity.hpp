#ifndef __LWM2M_CALLABLE_ENTITY_HPP
#define __LWM2M_CALLABLE_ENTITY_HPP

#include "ElementID.hpp"
#include "Endpoint.hpp"
#include "Requester.hpp"

namespace LwM2M {

struct CallableEntity {
  CallableEntity(RequesterPtr requester, EndpointPtr endpoint, ElementID id)
      : requester_(requester), endpoint_(endpoint), id_(id) {}

  EndpointPtr getEndpoint() { return endpoint_; }

  ElementID getID() { return id_; }

protected:
  RequesterPtr requester_;
  EndpointPtr endpoint_;
  ElementID id_;
};

using CallableEntityPtr = std::shared_ptr<CallableEntity>();
} // namespace LwM2M

#endif //__LWM2M_CALLABLE_ENTITY_HPP