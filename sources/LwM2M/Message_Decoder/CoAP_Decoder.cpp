#include "CoAP_Decoder.hpp"
#include "CoAP_Option.hpp"
#include "CoRE_Link.hpp"
#include "LoggerRepository.hpp"
#include "PlainText.hpp"
#include "StringSpliter.hpp"

#include <cctype>
#include <stdexcept>
#include <string>
#include <unordered_map>

using namespace std;
using namespace CoAP;
using namespace HaSLL;

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

unique_ptr<Register_Request> makeRegisterMessage(const CoAP::Message *input) {
  unique_ptr<Register_Request> result;
  if (input->getHeader().getCodeType() == CodeType::POST) {
    for (auto option : input->getOptions()) {
      if (option->getOptionNumber() == OptionNumber::URI_PATH) {
        if (option->getAsString() == "rd") {
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
              if (option->getAsString().substr(0, 2) == "b=") {
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

              string lwm2M_version_tag = option->getAsString().substr(0, 6);
              if (lwm2M_version_tag == "lwm2m=") {
                string version_string = option->getAsString().substr(6, 7);
                if (version_string == "1.0") {
                  version_ = LwM2M_Version::V1_0;
                } else if (version_string == "1.1") {
                  version_ = LwM2M_Version::V1_1;
                } else {
                  version_ = LwM2M_Version::UNRECOGNIZED;
                }
                continue;
              }

              string lifetime_tag = option->getAsString().substr(0, 3);
              if (lifetime_tag == "lt=") {
                life_time_ = atoll(option->getAsString().substr(4).c_str());
                continue;
              }

              string endpoint_tag = option->getAsString().substr(0, 3);
              if (endpoint_tag == "ep=") {
                endpoint_name_ = option->getAsString().substr(3);
                continue;
              }

              string sms_tag = option->getAsString().substr(0, 4);
              if (sms_tag == "sms=") {
                sms_number_ = option->getAsString().substr(4);
                continue;
              }

              string queue_tag = option->getAsString().substr(0, 1);
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

unique_ptr<Deregister_Request>
makeDeRegisterMessage(const CoAP::Message *input) {}
unique_ptr<Update_Request> makeUpdateMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeReadMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeWriteMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeExecuteMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeCreateMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeDeleteMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeWriteAttributesMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeDiscoverMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeReadCompositeMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeWriteCompositeMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeObserveMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeObserveCompositeMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeCancelObersvationMessage(const CoAP::Message *input) {}
unique_ptr<Message>
makeCancelObersvationCompositeMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeNotifyMessage(const CoAP::Message *input) {}
unique_ptr<Message> makeSendMessage(const CoAP::Message *input) {}

CoAP_Decoder::CoAP_Decoder(
    shared_ptr<RegistrationInterface> registration,
    shared_ptr<ThreadsafeUniqueQueue<CoAP::Message>> message_buffer)
    : registration_(registration), message_buffer_(message_buffer),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

bool CoAP_Decoder::processIfBootrstrapInterface(const CoAP::Message *message) {
  return false;
}

bool CoAP_Decoder::processIfDeviceRegistrationInteraface(
    const CoAP::Message *message) {
  bool result = false;
  for (auto option : message->getOptions()) {
    if (option->getOptionNumber() == OptionNumber::URI_PATH) {
      string uri_path = option->getAsString();
      if (uri_path == "rd") {
        result = registration_->handleRequest(makeRegisterMessage(message));
        break;
      } else if (uri_path.size() > 2) {
        if (uri_path.substr(0, 3) == "rd/") {
          switch (message->getHeader().getCodeType()) {
          case CodeType::POST: {
            result = registration_->handleRequest(makeUpdateMessage(message));
            break;
          }
          case CodeType::DELETE: {
            result =
                registration_->handleRequest(makeDeRegisterMessage(message));
            break;
          }
          default: { break; }
          }
        }
      }
    }
  }
  return result;
}

bool CoAP_Decoder::processIfDeviceManagmentInterface(
    const CoAP::Message *message) {
  return false;
}

bool CoAP_Decoder::processIfInformationReportingInterface(
    const CoAP::Message *message) {
  return false;
}

void CoAP_Decoder::decode(unique_ptr<CoAP::Message> message) {
  if (message) {
    if (processIfBootrstrapInterface(message.get())) {
      return;
    } else if (processIfDeviceRegistrationInteraface(message.get())) {
      return;
    } else if (processIfDeviceManagmentInterface(message.get())) {
      return;
    } else if (processIfInformationReportingInterface(message.get())) {
      return;
    }
  }
}

void CoAP_Decoder::run() {
  while (!stopRequested()) {
    try {
      decode(message_buffer_->try_pop());
    } catch (exception &ex) {
      logger_->log(HaSLL::SeverityLevel::ERROR, "Caught an exception: {}",
                   ex.what());
    }
  }
}
} // namespace LwM2M
