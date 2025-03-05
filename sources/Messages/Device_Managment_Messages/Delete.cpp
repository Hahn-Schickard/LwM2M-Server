#include "Delete.hpp"

using namespace std;

namespace LwM2M {
DeleteRequest::DeleteRequest(EndpointPtr endpoint, // NOLINT
    ElementID target_id)
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::Delete, make_shared<Payload>(target_id)) {}

string DeleteRequest::name() const { return "DeleteRequest"; }
}; // namespace LwM2M
