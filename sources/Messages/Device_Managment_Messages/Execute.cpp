#include "Execute.hpp"
#include "Variant_Visitor.hpp"

using namespace std;

namespace LwM2M {

ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, ElementID target)
    : ServerRequest(endpoint, MessageType::EXECUTE,
                    InterfaceType::DEVICE_MANAGEMENT,
                    make_shared<Payload>(target)) {}

ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, ElementID target,
                               string arguments)
    : ServerRequest(
          endpoint, MessageType::EXECUTE, InterfaceType::DEVICE_MANAGEMENT,
          make_shared<Payload>(make_pair(
              target, make_shared<DataFormat>(DataVariant(arguments))))) {}

string ExecuteRequest::name() { return "ExecuteRequest"; }
} // namespace LwM2M
