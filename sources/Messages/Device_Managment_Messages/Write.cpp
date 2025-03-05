#include "Write.hpp"

using namespace std;

namespace LwM2M {

WriteRequest::WriteRequest(EndpointPtr endpoint, // NOLINT
    ElementID target_id, DataFormatPtr content) // NOLINT
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::Write,
          make_shared<Payload>(make_pair(target_id, content))) {}

string WriteRequest::name() const { return "WriteRequest"; }
} // namespace LwM2M
