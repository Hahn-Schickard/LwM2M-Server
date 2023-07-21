#include "Execute.hpp"
#include "Variant_Visitor.hpp"

using namespace std;

namespace LwM2M {

ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, // NOLINT
    ElementID target)
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::EXECUTE, make_shared<Payload>(target)) {}

ExecuteRequest::ExecuteRequest(EndpointPtr endpoint, // NOLINT
    ElementID target, string arguments)
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::EXECUTE,
          make_shared<Payload>(make_pair(
              target, make_shared<DataFormat>(DataVariant(arguments))))) {}

string ExecuteRequest::name() const { return "ExecuteRequest"; }
} // namespace LwM2M
