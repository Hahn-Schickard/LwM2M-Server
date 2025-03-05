#include "ReadComposite.hpp"

using namespace std;

namespace LwM2M {

ReadCompositeRequest::ReadCompositeRequest(EndpointPtr endpoint, // NOLINT
    ElementIDs targets) // NOLINT
    : DeviceManagementRequest(endpoint, // NOLINT
          MessageType::Read_Composite, make_shared<Payload>(targets)) {}

string ReadCompositeRequest::name() const { return "ReadCompositeRequest"; }
} // namespace LwM2M
