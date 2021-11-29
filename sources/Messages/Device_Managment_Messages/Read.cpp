#include "Read.hpp"

using namespace std;

namespace LwM2M {

ReadRequest::ReadRequest(EndpointPtr endpoint, ElementID target_id)
    : DeviceManagementRequest(endpoint, MessageType::READ,
                              make_shared<Payload>(target_id)),
      target_(target_id) {}

string ReadRequest::name() { return "ReadRequest"; }

} // namespace LwM2M
