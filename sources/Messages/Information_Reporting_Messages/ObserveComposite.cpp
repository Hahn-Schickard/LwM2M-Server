#include "ObserveComposite.hpp"

using namespace std;

namespace LwM2M {
ObserveCompositeRequest::ObserveCompositeRequest(
    EndpointPtr endpoint, vector<ElmentIdVariant> target_ids)
    : ServerRequest(endpoint, MessageType::OBSERVE_COMPOSITE,
                    InterfaceType::INFORMATION_REPORTING,
                    make_shared<Payload>(target_ids)) {}

string ObserveCompositeRequest::name() { return "ObserveCompositeRequest"; }
} // namespace LwM2M
