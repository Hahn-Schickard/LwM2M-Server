#include "Write.hpp"

using namespace std;

namespace LwM2M {

WriteRequest::WriteRequest(EndpointPtr endpoint, ElmentIdVariant target_id,
                           DataFormatPtr content)
    : ServerRequest(endpoint, MessageType::WRITE,
                    InterfaceType::DEVICE_MANAGEMENT,
                    make_shared<Payload>(make_pair(target_id, content))) {}

string WriteRequest::name() { return "WriteRequest"; }
} // namespace LwM2M
