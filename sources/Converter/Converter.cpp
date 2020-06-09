#include "Converter.hpp"
#include "RegistrationInterfaceMessages.hpp"

#include "CoRE_Link.hpp"
#include "PlainText.hpp"

#include <cctype>
#include <optional>
#include <string>

using namespace std;
using namespace CoAP;
namespace LwM2M_Model {

unordered_map<unsigned int, unsigned int>
getObjectList(shared_ptr<PayloadFormat> payload) {
  unordered_map<unsigned int, unsigned int> result;
  switch (payload->getContentFormatType()) {
  case ContentFormatType::PLAIN_TEXT: {
    static_pointer_cast<PlainText>(payload);
    break;
  }
  case ContentFormatType::CORE_LINK: {
    break;
  }
  case ContentFormatType::OPAQUE: {
    break;
  }
  case ContentFormatType::CBOR: {
    break;
  }
  case ContentFormatType::JSON: {
    break;
  }
  case ContentFormatType::SENML_CBOR: {
    break;
  }
  case ContentFormatType::SENML_JSON: {
    break;
  }
  case ContentFormatType::TLV: {
    break;
  }
  case ContentFormatType::UNRECOGNIZED:
  default: { break; }
  }
  return result;
}

unique_ptr<LwM2M_Message> makeRegisterMessage(shared_ptr<CoAP_Message> input) {
  unique_ptr<LwM2M_Message> result;
  if (input->getHeader().getCodeType() == CodeType::POST) {
    for (auto option : input->getOptions()) {
      if (option->getOptionNumber() == OptionNumber::URI_PATH) {
        if (option->getValue() == "rd") {
          string endpoint_name_;
          size_t life_time_ = 0;
          LwM2M_Version version_ = LwM2M_Version::UNRECOGNIZED;
          BindingType binding_ = BindingType::MALFORMED;
          bool queue_mode_ = false;
          optional<string> sms_number_ = nullopt;
          unordered_map<unsigned int, unsigned int> object_instances_map_ =
              getObjectList(input->getBody());
          for (auto option : input->getOptions()) {
            if (option->getOptionNumber() == OptionNumber::URI_QUERY) {
              if (option->getValue().substr(0, 2) == "b=") {
                switch (toupper(option->getValue().at(3))) {
                case 'U': {
                  binding_ = BindingType::UDP;
                  break;
                }
                case 'T': {
                  binding_ = BindingType::TCP;
                  break;
                }
                case 'S': {
                  binding_ = BindingType::SMS;
                  break;
                }
                case 'N': {
                  binding_ = BindingType::NON_IP;
                  break;
                }
                default: { break; }
                }
              }

              if (option->getValue().substr(0, 6) == "lwm2m=") {
                string version_string = option->getValue().substr(7);
                if (version_string == "1.0") {
                  version_ = LwM2M_Version::V1_0;
                } else if (version_string == "1.1") {
                  version_ = LwM2M_Version::V1_1;
                } else {
                  version_ = LwM2M_Version::UNRECOGNIZED;
                }
              }

              if (option->getValue().substr(0, 3) == "lt=") {
                life_time_ = atoll(option->getValue().substr(4).c_str());
              }

              if (option->getValue().substr(0, 3) == "ep=") {
                endpoint_name_ = option->getValue().substr(3);
              }

              if (option->getValue().substr(0, 4) == "sms=") {
                sms_number_ = option->getValue().substr(4);
              }

              if (option->getValue().substr(0, 1) == "Q") {
                queue_mode_ = true;
              }
            }
          }

          result = make_unique<Register_Request>(
              endpoint_name_, input->getReceiverIP(), input->getReceiverPort(),
              life_time_, version_, binding_, queue_mode_, sms_number_,
              object_instances_map_);
        }
      }
    }
  }
  return result;
}

unique_ptr<LwM2M_Message>
makeDeRegisterMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message> makeUpdateMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message> makeReadMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message> makeWriteMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message> makeExecuteMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message> makeCreateMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message> makeDeleteMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeWriteAttributesMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message> makeDiscoverMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeReadCompositeMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeWriteCompositeMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message> makeObserveMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeObserveCompositeMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeCancelObersvationMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message>
makeCancelObersvationCompositeMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message> makeNotifyMessage(shared_ptr<CoAP_Message> input) {}
unique_ptr<LwM2M_Message> makeSendMessage(shared_ptr<CoAP_Message> input) {}

unique_ptr<LwM2M_Message> convert(shared_ptr<CoAP_Message> input) {
  unique_ptr<LwM2M_Message> result;
  if (!input->getOptions().empty()) {
    for (auto option : input->getOptions()) {
      if (option->getOptionNumber() == OptionNumber::URI_PATH) {
        string uri_path = option->getValue();
        if (uri_path == "rd") {
          result = makeRegisterMessage(input);
        } else if (uri_path.size() > 2) {
          if (uri_path.substr(0, 3) == "rd/") {
            switch (input->getHeader().getCodeType()) {
            case CodeType::POST: {
              result = makeUpdateMessage(input);
              break;
            }
            case CodeType::DELETE: {
              result = makeDeRegisterMessage(input);
              break;
            }
            default: {
              // log warning
              break;
            }
            }
          }
        }
      }
    }
    return result;
  }
}

CoAP_Message &makeFromRegisterMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromDeRegisterMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromUpdateMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromReadMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromWriteMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromExecuteMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromCreateMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromDeleteMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromWriteAttributesMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromDiscoverMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromReadCompositeMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromWriteCompositeMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromObserveMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromObserveCompositeMessage(unique_ptr<LwM2M_Message> input) {
}
CoAP_Message &
makeFromCancelObersvationMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &
makeFromCancelObersvationCompositeMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromNotifyMessage(unique_ptr<LwM2M_Message> input) {}
CoAP_Message &makeFromSendMessage(unique_ptr<LwM2M_Message> input) {}

CoAP_Message &convert(unique_ptr<LwM2M_Message> input);

} // namespace LwM2M_Model
