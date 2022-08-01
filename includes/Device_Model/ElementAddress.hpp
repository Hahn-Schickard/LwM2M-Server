#ifndef __LWM2M_CALLABLE_ENTITY_HPP
#define __LWM2M_CALLABLE_ENTITY_HPP

#include "ElementID.hpp"
#include "Endpoint.hpp"

namespace LwM2M {

/**
 * @brief Models a single addressable Device Model element
 *
 */
struct ElementAddress {
  ElementAddress(EndpointPtr endpoint, ElementID id);

  EndpointPtr getEndpoint();
  ElementID getID();

protected:
  EndpointPtr endpoint_;
  ElementID id_;
};

using ElementAddressPtr = std::shared_ptr<ElementAddress>();
} // namespace LwM2M

#endif //__LWM2M_CALLABLE_ENTITY_HPP