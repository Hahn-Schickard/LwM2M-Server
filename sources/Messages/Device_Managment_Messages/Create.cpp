#include "Create.hpp"

using namespace std;

namespace LwM2M {

CreateRequest::CreateRequest(EndpointPtr endpoint, ObjectID target_id,
                             DataFormatPtr content)
    : ServerRequest(endpoint, MessageType::CREATE,
                    InterfaceType::DEVICE_MANAGMENT,
                    make_shared<Payload>(
                        make_pair(ElmentIdVariant(target_id), content))) {}

string CreateRequest::name() { return "CreateRequest"; }
} // namespace LwM2M
