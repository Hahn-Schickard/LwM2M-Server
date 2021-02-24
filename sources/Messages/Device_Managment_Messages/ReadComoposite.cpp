#include "ReadComoposite.hpp"

using namespace std;

namespace LwM2M {

ReadComopositeRequest::ReadComopositeRequest(EndpointPtr endpoint,
                                             vector<ElmentIdVariant> targets)
    : ServerRequest(endpoint, MessageType::READ_COMPOSITE,
                    InterfaceType::DEVICE_MANAGMENT,
                    make_shared<Payload>(targets)) {}

string ReadComopositeRequest::name() { return "ReadComopositeRequest"; }
} // namespace LwM2M
