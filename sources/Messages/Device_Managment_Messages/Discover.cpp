#include "Discover.hpp"

using namespace std;

namespace LwM2M {

DiscoverRequest::DiscoverRequest(EndpointPtr endpoint, ElementID target_id)
    : DeviceManagementRequest(endpoint, MessageType::DISCOVER,
                              make_shared<Payload>(target_id)),
      target_(target_id) {}

string DiscoverRequest::name() { return "DiscoverRequest"; }
} // namespace LwM2M
