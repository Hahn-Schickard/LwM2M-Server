#include "CoAP_Decoder.hpp"
#include "CoAP_Option.hpp"
#include "CoRE_Link.hpp"
#include "LoggerRepository.hpp"
#include "PlainText.hpp"
#include "RegistrationMessages.hpp"
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
  if (payload) {
    switch (payload->getContentFormatType().getContentFormatType()) {
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
          payload->getContentFormatType().getAsString() + " fortmat type.";
      throw domain_error(move(error_msg));
    }
    }
  }
  return result;
}

unique_ptr<Register_Request> makeRegisterMessage(const CoAP::Message *input) {
  unique_ptr<Register_Request> result;
  for (auto option : input->getOptions()) {
    if (option->getOptionNumber() == OptionNumber::URI_PATH) {
      if (option->getAsString() == "rd") {
        string endpoint_name_;
        size_t life_time_ = 0;
        LwM2M_Version version_ = LwM2M_Version::UNRECOGNIZED;
        BindingType binding_ = BindingType::MALFORMED;
        bool queue_mode_ = false;
        string sms_number_;
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
  return result;
}

unique_ptr<Deregister_Request>
makeDeRegisterMessage(const CoAP::Message *input) {
  return make_unique<Deregister_Request>(
      input->getReceiverIP(), input->getReceiverPort(),
      input->getHeader().getMessageID(), input->getToken(),
      input->getOptions().at(1)->getAsString());
}

unique_ptr<Update_Request> makeUpdateMessage(const CoAP::Message *input) {
  auto options = input->getOptions();
  string location = options.at(1)->getAsString();
  optional<size_t> life_time_ = nullopt;
  optional<BindingType> binding_ = nullopt;
  optional<string> sms_number_ = nullopt;
  unordered_map<unsigned int, unsigned int> object_instances_map =
      getObjectList(input->getBody());
  for (auto it = options.begin() + 2; it != options.end(); it++) {
    if ((*it)->getOptionNumber() == OptionNumber::URI_QUERY) {
      if ((*it)->getAsString().substr(0, 2) == "b=") {
        auto binding_type = toupper((*it)->getValue().at(2));
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

      string lifetime_tag = (*it)->getAsString().substr(0, 3);
      if (lifetime_tag == "lt=") {
        life_time_ = atoll((*it)->getAsString().substr(4).c_str());
        continue;
      }

      string sms_tag = (*it)->getAsString().substr(0, 4);
      if (sms_tag == "sms=") {
        sms_number_ = (*it)->getAsString().substr(4);
        continue;
      }
    }
  }
  return make_unique<Update_Request>(
      input->getReceiverIP(), input->getReceiverPort(),
      input->getHeader().getMessageID(), input->getToken(), location,
      life_time_, binding_, sms_number_, object_instances_map);
}

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

ResponseCode convert(CoAP::CodeType code_type) {
  switch (code_type) {
  case CoAP::CodeType::OK: {
    return ResponseCode::OK;
  }
  case CoAP::CodeType::CREATED: {
    return ResponseCode::CREATED;
  }
  case CoAP::CodeType::DELETED: {
    return ResponseCode::DELETED;
  }
  case CoAP::CodeType::CHANGED: {
    return ResponseCode::CHANGED;
  }
  case CoAP::CodeType::CONTENT: {
    return ResponseCode::CONTENT;
  }
  case CoAP::CodeType::CONTINUE: {
    return ResponseCode::CONTINUE;
  }
  case CoAP::CodeType::BAD_REQUEST: {
    return ResponseCode::BAD_REQUEST;
  }
  case CoAP::CodeType::UNAUTHORIZED: {
    return ResponseCode::UNAUTHORIZED;
  }
  case CoAP::CodeType::FORBIDDEN: {
    return ResponseCode::FORBIDDEN;
  }
  case CoAP::CodeType::NOT_FOUND: {
    return ResponseCode::NOT_FOUND;
  }
  case CoAP::CodeType::METHOD_NOT_ALLOWED: {
    return ResponseCode::METHOD_NOT_ALLOWED;
  }
  case CoAP::CodeType::NOT_ACCEPTABLE: {
    return ResponseCode::NOT_ACCEPTABLE;
  }
  case CoAP::CodeType::REQUEST_ENTITY_INCOMPLETE: {
    return ResponseCode::REQUEST_ENTITY_TOO_LARGE;
  }
  case CoAP::CodeType::PRECOGNITION_FAILED: {
    return ResponseCode::PRECOGNITION_FAILED;
  }
  case CoAP::CodeType::REQUEST_ENTITY_TOO_LARGE: {
    return ResponseCode::REQUEST_ENTITY_TOO_LARGE;
  }
  case CoAP::CodeType::UNSUPPORTED_CONTENT_FORMAT: {
    return ResponseCode::UNSUPPORTED_CONTENT_FORMAT;
  }
  default: { return ResponseCode::UNHANDLED; }
  }
}

unique_ptr<Response> makeResponse(const CoAP::Message *message) {
  unique_ptr<Response> response;
  if (message->getHeader().getCodeType() == CoAP::CodeType::CONTENT) {
    response = make_unique<Response>(
        message->getReceiverIP(), message->getReceiverPort(),
        message->getHeader().getMessageID(), message->getToken(),
        MessageType::NOT_RECOGNIZED, ResponseCode::CONTENT,
        message->getBody()->getBytes());
  } else if (message->getHeader().getCodeType() == CoAP::CodeType::CONTINUE) {
    // @TODO: handle segmented packets
  } else if (message->getHeader().getCodeType() != CoAP::CodeType::UNHANDLED) {
    response = make_unique<Response>(
        message->getReceiverIP(), message->getReceiverPort(),
        message->getHeader().getMessageID(), message->getToken(),
        MessageType::NOT_RECOGNIZED,
        convert(message->getHeader().getCodeType()));
  }
  return response;
}

CoAP_Decoder::CoAP_Decoder(
    shared_ptr<RegistrationInterface> registration,
    shared_ptr<ThreadsafeUniqueQueue<CoAP::Message>> message_buffer,
    shared_ptr<ResponseHandler> response_handler)
    : registration_(registration), message_buffer_(message_buffer),
      response_handler_(response_handler),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

bool CoAP_Decoder::processIfResponse(const CoAP::Message *message) {
  if (message->getHeader().getMesageType() ==
      CoAP::MessageType::ACKNOWLEDGMENT) {
    auto response = makeResponse(message);
    if (response) {
      response_handler_->setResponse(move(response));
      return true;
    }
  }
  return false;
}

bool CoAP_Decoder::processIfBootrstrapInterface(const CoAP::Message *message) {
  return false;
}

bool CoAP_Decoder::processIfDeviceRegistrationInteraface(
    const CoAP::Message *message) {
  auto options = message->getOptions();
  for (auto it = options.begin(); it != options.end(); it++) {
    if ((*it)->getOptionNumber() == OptionNumber::URI_PATH) {
      if ((*it)->getAsString() == "rd") {
        if (message->getHeader().getCodeType() == CodeType::POST) {
          auto next_option = it + 1;
          if (next_option != options.end()) {
            if ((*next_option)->getOptionNumber() == OptionNumber::URI_PATH)
              return registration_->handleRequest(makeUpdateMessage(message));
          }
          return registration_->handleRequest(makeRegisterMessage(message));
        } else if (message->getHeader().getCodeType() == CodeType::DELETE) {
          return registration_->handleRequest(makeDeRegisterMessage(message));
        }
      }
    }
  }
  return false;
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
    if (processIfResponse(message.get())) {
    } else if (processIfBootrstrapInterface(message.get())) {
    } else if (processIfDeviceRegistrationInteraface(message.get())) {
    } else if (processIfDeviceManagmentInterface(message.get())) {
    } else if (processIfInformationReportingInterface(message.get())) {
    }
  }
} // namespace LwM2M

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
