#include "Discover.hpp"

using namespace std;

namespace LwM2M {

DiscoverRequest::DiscoverRequest(EndpointPtr endpoint,
                                 ElmentIdVariant target_id)
    : ServerRequest(endpoint, MessageType::DISCOVER,
                    InterfaceType::DEVICE_MANAGEMENT,
                    make_shared<Payload>(ElmentIdVariant(target_id))) {}

string DiscoverRequest::name() { return "DiscoverRequest"; }
} // namespace LwM2M
