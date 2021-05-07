#include "WriteAttributes.hpp"

using namespace std;

namespace LwM2M {

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint)
    : ServerRequest(endpoint, MessageType::WRITE_ATTRIBUTES,
                    InterfaceType::DEVICE_MANAGEMENT) {}

WriteAttributesRequest::WriteAttributesRequest(EndpointPtr endpoint,
                                               vector<TargetAttribute> content)
    : ServerRequest(endpoint, MessageType::WRITE_ATTRIBUTES,
                    InterfaceType::DEVICE_MANAGEMENT,
                    make_shared<Payload>(content)) {}

string WriteAttributesRequest::name() { return "WriteAttributesRequest"; }
} // namespace LwM2M
