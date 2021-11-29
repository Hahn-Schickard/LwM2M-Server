#include "Delete.hpp"

using namespace std;

namespace LwM2M {
DeleteRequest::DeleteRequest(EndpointPtr endpoint, ElementID target_id)
    : DeviceManagementRequest(endpoint, MessageType::DELETE,
                              make_shared<Payload>(target_id)) {}

string DeleteRequest::name() { return "DeleteRequest"; }
}; // namespace LwM2M
