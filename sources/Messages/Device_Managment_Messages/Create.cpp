#include "Create.hpp"

using namespace std;

namespace LwM2M {

CreateRequest::CreateRequest(EndpointPtr endpoint, // NOLINT
    ElementID target_id, DataFormatPtr content) // NOLINT
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::CREATE,
          make_shared<Payload>(make_pair(target_id, content))) {}

string CreateRequest::name() const { return "CreateRequest"; }
} // namespace LwM2M
