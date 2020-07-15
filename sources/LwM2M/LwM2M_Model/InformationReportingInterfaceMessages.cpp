#include "InformationReportingInterfaceMessages.hpp"

using namespace std;

namespace LwM2M {

Observe_Request::Observe_Request(string endpoint_address,
                                 unsigned int endpoint_port,
                                 vector<uint8_t> token, unsigned int object_id,
                                 optional<unsigned int> object_instance_id,
                                 optional<unsigned int> resource_id,
                                 optional<unsigned int> resoruce_instance_id)
    : InformationReporting_Interface_Message(endpoint_address, endpoint_port,
                                             token, MessageType::OBSERVE),
      object_id_(object_id), object_instance_id_(move(object_instance_id)),
      resource_id_(move(resource_id)),
      resoruce_instance_id_(move(resoruce_instance_id)) {}

Notify_Request::Notify_Request(string endpoint_address,
                               unsigned int endpoint_port,
                               vector<uint8_t> token, string value)
    : InformationReporting_Interface_Message(endpoint_address, endpoint_port,
                                             token, MessageType::NOTIFY),
      value_(move(value)) {}

CancelObservation_Request::CancelObservation_Request(string endpoint_address,
                                                     unsigned int endpoint_port,
                                                     vector<uint8_t> token,
                                                     string location)
    : InformationReporting_Interface_Message(endpoint_address, endpoint_port,
                                             token,
                                             MessageType::CANCEL_OBSERVATION),
      location_(move(location)) {}

ObserveComposite_Request::ObserveComposite_Request(string endpoint_address,
                                                   unsigned int endpoint_port,
                                                   vector<uint8_t> token)
    : InformationReporting_Interface_Message(endpoint_address, endpoint_port,
                                             token,
                                             MessageType::OBSERVE_COMPOSITE) {
  throw UnsupportedOperation("Observe-Composite operation is not supported!");
}

CancelObserveComposite_Request::CancelObserveComposite_Request(
    string endpoint_address, unsigned int endpoint_port, vector<uint8_t> token)
    : InformationReporting_Interface_Message(
          endpoint_address, endpoint_port, token,
          MessageType::CANCEL_OBSERVATION_COMPOSITE) {
  throw UnsupportedOperation(
      "Cancel Observe-Composite operation is not supported!");
}

Send_Request::Send_Request(string endpoint_address, unsigned int endpoint_port,
                           vector<uint8_t> token)
    : InformationReporting_Interface_Message(endpoint_address, endpoint_port,
                                             token, MessageType::SEND) {
  throw UnsupportedOperation("Send operation is not supported!");
}

} // namespace LwM2M
