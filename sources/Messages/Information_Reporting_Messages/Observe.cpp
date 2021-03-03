#include "Observe.hpp"

using namespace std;

namespace LwM2M {

ObserveRequest::ObserveRequest(EndpointPtr endpoint, ElmentIdVariant target_id)
    : ServerRequest(endpoint, MessageType::OBSERVE,
                    InterfaceType::INFORMATION_REPORTING,
                    make_shared<Payload>(target_id)) {}

string ObserveRequest::name() { return "ObserveRequest"; }
} // namespace LwM2M
