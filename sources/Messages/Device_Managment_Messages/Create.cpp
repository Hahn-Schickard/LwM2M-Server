#include "Create.hpp"

using namespace std;

namespace LwM2M {

CreateRequest::CreateRequest(EndpointPtr endpoint, ElementID target_id,
                             DataFormatPtr content)
    : DeviceManagementRequest(
          endpoint, MessageType::CREATE,
          make_shared<Payload>(make_pair(target_id, content))) {}

string CreateRequest::name() { return "CreateRequest"; }
} // namespace LwM2M
