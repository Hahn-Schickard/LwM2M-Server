#include "WriteComposite.hpp"

using namespace std;

namespace LwM2M {

WriteCompositeRequest::WriteCompositeRequest(EndpointPtr endpoint,
                                             vector<TargetContent> content)
    : ServerRequest(endpoint, MessageType::WRITE_COMPOSITE,
                    InterfaceType::DEVICE_MANAGEMENT,
                    make_shared<Payload>(content)) {}

string WriteCompositeRequest::name() { return "WriteCompositeRequest"; }
} // namespace LwM2M
