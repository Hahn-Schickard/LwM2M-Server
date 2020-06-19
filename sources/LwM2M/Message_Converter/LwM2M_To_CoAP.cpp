#include "LwM2M_To_CoAP.hpp"

using namespace std;
using namespace CoAP;
namespace LwM2M_Model {

CoAP_Message &makeFromRegisterMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromDeRegisterMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromUpdateMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromReadMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromWriteMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromExecuteMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromCreateMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromDeleteMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromWriteAttributesMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromDiscoverMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromReadCompositeMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromWriteCompositeMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromObserveMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromObserveCompositeMessage(shared_ptr<LwM2M_Message> input) {
}
CoAP_Message &
makeFromCancelObersvationMessage(shared_ptr<LwM2M_Message> input) {}
CoAP_Message &
makeFromCancelObersvationCompositeMessage(shared_ptr<LwM2M_Message> input) {}

LwM2M_To_CoAP::LwM2M_To_CoAP(
    shared_ptr<ThreadsafeQueue<CoAP_Message>> output_queue)
    : output_queue_(output_queue) {}

void LwM2M_To_CoAP::convert(shared_ptr<LwM2M_Message> message) {
  switch (message->message_type_) {
  case MessageType::REGISTER: {
    output_queue_->push(makeFromRegisterMessage(message));
    break;
  }
  case MessageType::DEREGISTER: {
    output_queue_->push(makeFromDeRegisterMessage(message));
    break;
  }
  case MessageType::UPDATE: {
    output_queue_->push(makeFromUpdateMessage(message));
    break;
  }
  case MessageType::READ: {
    output_queue_->push(makeFromReadMessage(message));
    break;
  }
  case MessageType::WRITE: {
    output_queue_->push(makeFromWriteMessage(message));
    break;
  }
  case MessageType::EXECUTE: {
    output_queue_->push(makeFromExecuteMessage(message));
    break;
  }
  case MessageType::CREATE: {
    output_queue_->push(makeFromCreateMessage(message));
    break;
  }
  case MessageType::DELETE: {
    output_queue_->push(makeFromDeleteMessage(message));
    break;
  }
  case MessageType::WRITE_ATTRIBUTES: {
    output_queue_->push(makeFromWriteAttributesMessage(message));
    break;
  }
  case MessageType::DISCOVER: {
    output_queue_->push(makeFromDiscoverMessage(message));
    break;
  }
  case MessageType::READ_COMPOSITE: {
    output_queue_->push(makeFromReadCompositeMessage(message));
    break;
  }
  case MessageType::WRITE_COMPOSITE: {
    output_queue_->push(makeFromWriteCompositeMessage(message));
    break;
  }
  case MessageType::OBSERVE: {
    output_queue_->push(makeFromObserveMessage(message));
    break;
  }
  case MessageType::OBSERVE_COMPOSITE: {
    output_queue_->push(makeFromObserveCompositeMessage(message));
    break;
  }
  case MessageType::CANCEL_OBSERVATION: {
    output_queue_->push(makeFromCancelObersvationMessage(message));
    break;
  }
  case MessageType::CANCEL_OBSERVATION_COMPOSITE: {
    output_queue_->push(makeFromCancelObersvationCompositeMessage(message));
    break;
  }
  case MessageType::NOTIFY: {
    // server does not issue notifications
  }
  case MessageType::SEND: {
    // server does not issue sends
  }
  default: { break; }
  }
}

} // namespace LwM2M_Model