#include "Observe.hpp"

using namespace std;

namespace LwM2M {

ObserveRequest::ObserveRequest(EndpointPtr endpoint, ElementID target_id)
    : InformationReportingRequest(endpoint, MessageType::OBSERVE,
                                  make_shared<Payload>(target_id)) {}

string ObserveRequest::name() { return "ObserveRequest"; }
} // namespace LwM2M
