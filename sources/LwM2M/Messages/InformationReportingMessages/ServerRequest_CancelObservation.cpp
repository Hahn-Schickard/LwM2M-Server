#include "ServerRequest_CancelObservation.hpp"

using namespace std;

namespace LwM2M {
ServerRequest_CancelObservation::ServerRequest_CancelObservation(
    string endpoint_address, unsigned int endpoint_port, uint16_t message_id,
    vector<uint8_t> token, string location)
    : InformationReportingMessage(endpoint_address, endpoint_port, message_id,
                                  token, MessageType::CANCEL_OBSERVATION),
      location_(move(location)) {}
} // namespace LwM2M