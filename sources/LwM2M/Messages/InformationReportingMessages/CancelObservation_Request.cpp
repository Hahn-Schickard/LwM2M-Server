#include "CancelObservation_Request.hpp"

using namespace std;

namespace LwM2M {
CancelObservation_Request::CancelObservation_Request(string endpoint_address,
                                                     unsigned int endpoint_port,
                                                     uint16_t message_id,
                                                     vector<uint8_t> token,
                                                     string location)
    : InformationReporting_Interface_Message(endpoint_address, endpoint_port,
                                             message_id, token,
                                             MessageType::CANCEL_OBSERVATION),
      location_(move(location)) {}
} // namespace LwM2M