#include "Write.hpp"

using namespace std;

namespace LwM2M {

WriteRequest::WriteRequest(EndpointPtr endpoint, // NOLINT
    ElementID target_id, DataFormatPtr content)
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::WRITE,
          make_shared<Payload>(make_pair(target_id, content))) {}

string WriteRequest::name() { return "WriteRequest"; }
} // namespace LwM2M
