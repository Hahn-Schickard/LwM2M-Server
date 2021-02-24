#include "WriteComoposite.hpp"

using namespace std;

namespace LwM2M {

WriteComopositeRequest::WriteComopositeRequest(EndpointPtr endpoint,
                                               vector<TargetContent> content)
    : ServerRequest(endpoint, MessageType::WRITE_COMPOSITE,
                    InterfaceType::DEVICE_MANAGMENT,
                    make_shared<Payload>(content)) {}

string WriteComopositeRequest::name() { return "WriteComopositeRequest"; }
} // namespace LwM2M
