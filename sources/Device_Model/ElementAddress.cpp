#include "ElementAddress.hpp"

namespace LwM2M {

ElementAddress::ElementAddress(EndpointPtr endpoint, // NOLINT
    ElementID id)
    : endpoint_(endpoint), // NOLINT
      id_(id) {}

EndpointPtr ElementAddress::getEndpoint() { return endpoint_; }

ElementID ElementAddress::getID() { return id_; }
} // namespace LwM2M
