#include "InformationReportingInterfaceMessages.hpp"

using namespace std;

namespace LwM2M_Model {

Observe_Request::Observe_Request(unsigned int object_id,
                                 optional<unsigned int> object_instance_id,
                                 optional<unsigned int> resource_id,
                                 optional<unsigned int> resoruce_instance_id)
    : InformationReporting_Interface_Message(MessageType::OBSERVE),
      object_id_(object_id), object_instance_id_(move(object_instance_id)),
      resource_id_(move(resource_id)),
      resoruce_instance_id_(move(resoruce_instance_id)) {}

Notify_Request::Notify_Request(string token, string value)
    : InformationReporting_Interface_Message(MessageType::NOTIFY),
      token_(move(token)), value_(move(value)) {}

CancelObservation_Request::CancelObservation_Request(string location)
    : InformationReporting_Interface_Message(MessageType::CANCEL_OBSERVATION),
      location_(move(location)) {}

ObserveComposite_Request::ObserveComposite_Request()
    : InformationReporting_Interface_Message(MessageType::OBSERVE_COMPOSITE) {
  throw UnsupportedOperation("Observe-Composite operation is not supported!");
}

CancelObserveComposite_Request::CancelObserveComposite_Request()
    : InformationReporting_Interface_Message(
          MessageType::CANCEL_OBSERVATION_COMPOSITE) {
  throw UnsupportedOperation(
      "Cancel Observe-Composite operation is not supported!");
}

Send_Request::Send_Request()
    : InformationReporting_Interface_Message(MessageType::SEND) {
  throw UnsupportedOperation("Send operation is not supported!");
}

} // namespace LwM2M_Model
