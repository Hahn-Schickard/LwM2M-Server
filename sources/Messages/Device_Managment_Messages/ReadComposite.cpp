#include "ReadComposite.hpp"

using namespace std;

namespace LwM2M {

ReadCompositeRequest::ReadCompositeRequest(EndpointPtr endpoint,
                                           ElementIDs targets)
    : DeviceManagementRequest(endpoint, MessageType::READ_COMPOSITE,
                              make_shared<Payload>(targets)) {}

string ReadCompositeRequest::name() { return "ReadCompositeRequest"; }
} // namespace LwM2M
