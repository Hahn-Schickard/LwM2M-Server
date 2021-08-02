#include "Create.hpp"

using namespace std;

namespace LwM2M {

CreateRequest::CreateRequest(EndpointPtr endpoint, ElementID target_id,
                             DataFormatPtr content)
    : ServerRequest(endpoint, MessageType::CREATE,
                    InterfaceType::DEVICE_MANAGEMENT,
                    make_shared<Payload>(make_pair(target_id, content))) {}

string CreateRequest::name() { return "CreateRequest"; }
} // namespace LwM2M
