#include "Execute.hpp"
#include "Variant_Visitor.hpp"

using namespace std;

namespace LwM2M {

ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, ResourceID target)
    : ServerRequest(endpoint, MessageType::EXECUTE,
                    InterfaceType::DEVICE_MANAGMENT,
                    make_shared<Payload>(ElmentIdVariant(target))) {}

ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, ResourceID target,
                               string arguments)
    : ServerRequest(endpoint, MessageType::EXECUTE,
                    InterfaceType::DEVICE_MANAGMENT,
                    make_shared<Payload>(make_pair(
                        ElmentIdVariant(target),
                        make_shared<DataFormat>(DataVariant(arguments))))) {}

string ExecuteRequest::name() { return "ExecuteRequest"; }
} // namespace LwM2M
