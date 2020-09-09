#include "ServerRequest_ObserveComposite.hpp"

using namespace std;

namespace LwM2M {
ServerRequest_ObserveComposite::ServerRequest_ObserveComposite(
    string endpoint_address, unsigned int endpoint_port, uint16_t message_id,
    vector<uint8_t> token)
    : InformationReporting_Interface_Message(endpoint_address, endpoint_port,
                                             message_id, token,
                                             MessageType::OBSERVE_COMPOSITE) {
  throw UnsupportedOperation("Observe-Composite operation is not supported!");
}
} // namespace LwM2M