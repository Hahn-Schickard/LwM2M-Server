#include "Converter.hpp"

#include <string>

using namespace std;
namespace LwM2M_Model {

unique_ptr<LwM2M_Message>
makeRegisterMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeDeRegisterMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeUpdateMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeReadMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeWriteMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeExecuteMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeCreateMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeDeleteMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeWriteAttributesMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeDiscoverMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeReadCompositeMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeWriteCompositeMessage(shared_ptr<CoAP::CoAP_Message> input) {}

unique_ptr<LwM2M_Message>
makeObserveMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeObserveCompositeMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeCancelObersvationMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeCancelObersvationCompositeMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeNotifyMessage(shared_ptr<CoAP::CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeSendMessage(shared_ptr<CoAP::CoAP_Message> input) {}

unique_ptr<LwM2M_Message>
convert(shared_ptr<shared_ptr<CoAP::CoAP_Message>> input) {

  return make_unique<LwM2M_Message>(InterfaceType::REGISTRATION,
                                    MessageType::REGISTER);
}

CoAP::CoAP_Message &makeFromRegisterMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &makeFromDeRegisterMessage(unique_ptr<LwM2M_Message> input) {
}
CoAP::CoAP_Message &makeFromUpdateMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &makeFromReadMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &makeFromWriteMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &makeFromExecuteMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &makeFromCreateMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &makeFromDeleteMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &
makeFromWriteAttributesMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &makeFromDiscoverMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &
makeFromReadCompositeMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &
makeFromWriteCompositeMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &makeFromObserveMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &
makeFromObserveCompositeMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &
makeFromCancelObersvationMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &
makeFromCancelObersvationCompositeMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &makeFromNotifyMessage(unique_ptr<LwM2M_Message> input) {}
CoAP::CoAP_Message &makeFromSendMessage(unique_ptr<LwM2M_Message> input) {}

CoAP::CoAP_Message &convert(unique_ptr<LwM2M_Message> input);

} // namespace LwM2M_Model
