#include "Discover.hpp"

using namespace std;

namespace LwM2M {

DiscoverRequest::DiscoverRequest(EndpointPtr endpoint, // NOLINT
    ElementID target_id)
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::DISCOVER, make_shared<Payload>(target_id)),
      target_(target_id) {}

string DiscoverRequest::name() const { return "DiscoverRequest"; }
} // namespace LwM2M
