#include "Delete.hpp"

using namespace std;

namespace LwM2M {
DeleteRequest::DeleteRequest(EndpointPtr endpoint, ObjectInstanceID target_id)
    : ServerRequest(endpoint, MessageType::DELETE,
                    InterfaceType::DEVICE_MANAGEMENT,
                    make_shared<Payload>(ElmentIdVariant(target_id))) {}

string DeleteRequest::name() { return "DeleteRequest"; }
}; // namespace LwM2M
