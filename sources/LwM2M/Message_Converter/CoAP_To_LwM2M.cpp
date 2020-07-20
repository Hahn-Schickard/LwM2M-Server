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
namespace LwM2M {

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

unique_ptr<Message> makeRegisterMessage(unique_ptr<CoAP::Message> input) {
  unique_ptr<Message> result;
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
                auto binding_type = toupper(option->getValue().at(2));
                switch (binding_type) {
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
                continue;
              }

              string lwm2M_version_tag = option->getValue().substr(0, 6);
              if (lwm2M_version_tag == "lwm2m=") {
                string version_string = option->getValue().substr(6, 7);
                if (version_string == "1.0") {
                  version_ = LwM2M_Version::V1_0;
                } else if (version_string == "1.1") {
                  version_ = LwM2M_Version::V1_1;
                } else {
                  version_ = LwM2M_Version::UNRECOGNIZED;
                }
                continue;
              }

              string lifetime_tag = option->getValue().substr(0, 3);
              if (lifetime_tag == "lt=") {
                life_time_ = atoll(option->getValue().substr(4).c_str());
                continue;
              }

              string endpoint_tag = option->getValue().substr(0, 3);
              if (endpoint_tag == "ep=") {
                endpoint_name_ = option->getValue().substr(3);
                continue;
              }

              string sms_tag = option->getValue().substr(0, 4);
              if (sms_tag == "sms=") {
                sms_number_ = option->getValue().substr(4);
                continue;
              }

              string queue_tag = option->getValue().substr(0, 1);
              if (queue_tag == "Q") {
                queue_mode_ = true;
                continue;
              }
            }
          }

          result = make_unique<Register_Request>(
              input->getReceiverIP(), input->getReceiverPort(),
              input->getHeader().getMessageID(), input->getToken(),
              endpoint_name_, life_time_, version_, binding_, queue_mode_,
              sms_number_, object_instances_map_);
        }
      }
    }
  }
  return result;
}

unique_ptr<Message> makeDeRegisterMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeUpdateMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeReadMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeWriteMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeExecuteMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeCreateMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeDeleteMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message>
makeWriteAttributesMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeDiscoverMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeReadCompositeMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeWriteCompositeMessage(unique_ptr<CoAP::Message> input) {
}
unique_ptr<Message> makeObserveMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message>
makeObserveCompositeMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message>
makeCancelObersvationMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message>
makeCancelObersvationCompositeMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeNotifyMessage(unique_ptr<CoAP::Message> input) {}
unique_ptr<Message> makeSendMessage(unique_ptr<CoAP::Message> input) {}

bool processIfBootrstrapInterface(
    shared_ptr<Option> option, unique_ptr<CoAP::Message> message,
    shared_ptr<ThreadsafeQueue<Message>> output_queue) {
  return false;
}

bool processIfDeviceRegistrationInteraface(
    shared_ptr<Option> option, unique_ptr<CoAP::Message> message,
    shared_ptr<ThreadsafeQueue<Message>> output_queue) {
  bool result = false;
  if (option->getOptionNumber() == OptionNumber::URI_PATH) {
    string uri_path = option->getValue();
    if (uri_path == "rd") {
      auto register_msg = makeRegisterMessage(move(message));
      if (register_msg) {
        output_queue->push(move(register_msg));
        result = true;
      }
    } else if (uri_path.size() > 2) {
      if (uri_path.substr(0, 3) == "rd/") {
        switch (message->getHeader().getCodeType()) {
        case CodeType::POST: {
          auto update_msg = makeUpdateMessage(move(message));
          if (update_msg) {
            output_queue->push(move(update_msg));
            result = true;
          }
          break;
        }
        case CodeType::DELETE: {
          auto deregister_msg = makeDeRegisterMessage(move(message));
          if (deregister_msg) {
            output_queue->push(move(deregister_msg));
            result = true;
          }
          break;
        }
        default: { break; }
        }
      }
    }
  }
  return result;
} // namespace LwM2M_Model

bool processIfDeviceManagmentInterface(
    shared_ptr<Option> option, unique_ptr<CoAP::Message> message,
    shared_ptr<ThreadsafeQueue<Message>> output_queue) {
  return false;
}

bool processIfInformationReportingInterface(
    shared_ptr<Option> option, unique_ptr<CoAP::Message> message,
    shared_ptr<ThreadsafeQueue<Message>> output_queue) {
  return false;
}

CoAP_To_LwM2M::CoAP_To_LwM2M(shared_ptr<ThreadsafeQueue<Message>> output_queue)
    : output_queue_(output_queue) {}

void CoAP_To_LwM2M::convert(unique_ptr<CoAP::Message> message) {
  if (message) {
    for (auto option : message->getOptions()) {
      if (processIfBootrstrapInterface(option, move(message), output_queue_)) {
        break;
      } else if (processIfDeviceRegistrationInteraface(option, move(message),
                                                       output_queue_)) {
        break;
      } else if (processIfDeviceManagmentInterface(option, move(message),
                                                   output_queue_)) {
        break;
      } else if (processIfInformationReportingInterface(option, move(message),
                                                        output_queue_)) {
        break;
      }
    }
  }
}
}; // namespace LwM2M