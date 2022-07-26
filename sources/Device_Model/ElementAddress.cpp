#include "ElementAddress.hpp"

namespace LwM2M {

ElementAddress::ElementAddress(EndpointPtr endpoint, ElementID id)
    : endpoint_(endpoint), id_(id) {}

EndpointPtr ElementAddress::getEndpoint() { return endpoint_; }

ElementID ElementAddress::getID() { return id_; }
} // namespace LwM2M
