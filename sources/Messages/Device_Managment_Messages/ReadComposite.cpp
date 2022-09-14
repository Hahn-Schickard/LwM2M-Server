#include "ReadComposite.hpp"

using namespace std;

namespace LwM2M {

ReadCompositeRequest::ReadCompositeRequest(EndpointPtr endpoint, // NOLINT
    ElementIDs targets)
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::READ_COMPOSITE, make_shared<Payload>(targets)) {}

string ReadCompositeRequest::name() { return "ReadCompositeRequest"; }
} // namespace LwM2M
