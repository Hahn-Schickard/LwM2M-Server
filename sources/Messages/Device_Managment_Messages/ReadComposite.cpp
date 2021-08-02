#include "ReadComposite.hpp"

using namespace std;

namespace LwM2M {

ReadCompositeRequest::ReadCompositeRequest(EndpointPtr endpoint,
                                           ElementIDs targets)
    : ServerRequest(endpoint, MessageType::READ_COMPOSITE,
                    InterfaceType::DEVICE_MANAGEMENT,
                    make_shared<Payload>(targets)) {}

string ReadCompositeRequest::name() { return "ReadCompositeRequest"; }
} // namespace LwM2M
