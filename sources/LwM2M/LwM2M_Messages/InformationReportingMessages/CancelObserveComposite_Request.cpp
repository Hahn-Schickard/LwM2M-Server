#include "CancelObserveComposite_Request.hpp"

using namespace std;

namespace LwM2M {
CancelObserveComposite_Request::CancelObserveComposite_Request(
    string endpoint_address, unsigned int endpoint_port, uint16_t message_id,
    vector<uint8_t> token)
    : InformationReporting_Interface_Message(
          endpoint_address, endpoint_port, message_id, token,
          MessageType::CANCEL_OBSERVATION_COMPOSITE) {
  throw UnsupportedOperation(
      "Cancel Observe-Composite operation is not supported!");
}
} // namespace LwM2M