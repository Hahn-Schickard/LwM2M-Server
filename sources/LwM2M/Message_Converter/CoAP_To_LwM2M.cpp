#include "CoAP_To_LwM2M.hpp"
#include "CoRE_Link.hpp"
#include "PlainText.hpp"
#include "RegistrationInterfaceMessages.hpp"
#include "StringSpliter.hpp"

#include <cctype>
#include <stdexcept>
#include <string>
#include <unordered_map>

using namespace std;
using namespace CoAP;
namespace LwM2M_Model {

unordered_map<unsigned int, unsigned int>
getObjectList(shared_ptr<PayloadFormat> payload) {
  unordered_map<unsigned int, unsigned int> result;
  switch (payload->getContentFormatType()) {
  case ContentFormatType::CORE_LINK: {
    shared_ptr<CoRE_Links> core_links =
        static_pointer_cast<CoRE_Links>(payload);
    for (auto link : core_links->getLinks()) {
      // ignore content attributes
      if (link.getTarget() != "/") {
        vector<string> object_instance_pair =
            utility::split(link.getTarget(), '/');
        if (object_instance_pair.size() == 2) {
          result.emplace(atoi(object_instance_pair.at(0).c_str()),
                         atoi(object_instance_pair.at(1).c_str()));
        }
      }
    }
    break;
  }
  default: {
    string error_msg =
        "Registration request must use " +
        toString(ContentFormatType::CORE_LINK) + " fortmat type, not " +
        toString(payload->getContentFormatType()) + " fortmat type.";
    throw domain_error(move(error_msg));
  }
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
              input->getReceiverIP(), input->getReceiverPort(),
              input->getToken(), endpoint_name_, life_time_, version_, binding_,
              queue_mode_, sms_number_, object_instances_map_);
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

bool processIfBootrstrapInterface(shared_ptr<CoAP_Option> option,
                                  shared_ptr<CoAP_Message> message) {
  return false;
}

bool processIfDeviceRegistrationInteraface(shared_ptr<CoAP_Option> option,
                                           shared_ptr<CoAP_Message> message) {
  unique_ptr<LwM2M_Message> result;
  if (option->getOptionNumber() == OptionNumber::URI_PATH) {
    string uri_path = option->getValue();
    if (uri_path == "rd") {
      result = makeRegisterMessage(message);
    } else if (uri_path.size() > 2) {
      if (uri_path.substr(0, 3) == "rd/") {
        switch (message->getHeader().getCodeType()) {
        case CodeType::POST: {
          result = makeUpdateMessage(message);
          break;
        }
        case CodeType::DELETE: {
          result = makeDeRegisterMessage(message);
          break;
        }
        default: { break; }
        }
      }
    }
  }
  return result ? true : false;
}

bool processIfDeviceManagmentInterface(shared_ptr<CoAP_Option> option,
                                       shared_ptr<CoAP_Message> message) {
  return false;
}

bool processIfInformationReportingInterface(shared_ptr<CoAP_Option> option,
                                            shared_ptr<CoAP_Message> message) {
  return false;
}

void CoAP_To_LwM2M::convert(shared_ptr<CoAP_Message> message) {
  if (!message->getOptions().empty()) {
    for (auto option : message->getOptions()) {
      if (processIfBootrstrapInterface(option, message)) {
      } else if (processIfDeviceRegistrationInteraface(option, message)) {
      } else if (processIfDeviceManagmentInterface(option, message)) {
      } else if (processIfInformationReportingInterface(option, message)) {
      }
    }
  }
}
}; // namespace LwM2M_Model