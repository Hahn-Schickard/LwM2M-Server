#include "CoAP_RegistrationMessageDecoder.hpp"
#include "CoAP_Option.hpp"
#include "CoRE_Link.hpp"
#include "LoggerRepository.hpp"
#include "StringSpliter.hpp"

#include <cctype>
#include <stdexcept>
#include <unordered_map>

using namespace std;
using namespace CoAP;
using namespace HaSLL;

namespace LwM2M {

pair<unsigned int, unsigned int> makeObjectInstancePair(CoRE_Link link) {
  vector<string> uri_targets = utility::split(link.getTarget(), '/');
  if (uri_targets.size() == 2) {
    return make_pair<unsigned int, unsigned int>(
        atoi(uri_targets.at(0).c_str()), atoi(uri_targets.at(1).c_str()));
  } else {
    string error_msg = "CoRE Link conatins more than 2 uri targets";
    throw logic_error(move(error_msg));
  }
}

void addToMap(unordered_map<unsigned int, vector<unsigned int>> &map,
              pair<unsigned int, unsigned int> instance) {
  auto it = map.find(instance.first);
  if (it != map.end()) {
    it->second.push_back(instance.second);
  } else {
    map.emplace(instance.first, vector<unsigned int>{instance.second});
  }
}

unordered_map<unsigned int, vector<unsigned int>>
getObjectList(shared_ptr<PayloadFormat> payload) {
  unordered_map<unsigned int, vector<unsigned int>> result;
  if (payload) {
    switch (payload->getContentFormatType().getContentFormatType()) {
    case CoAP::ContentFormatType::CORE_LINK: {
      auto core_links_payload = static_pointer_cast<CoRE_Links>(payload);
      for (auto core_link : core_links_payload->getLinks()) {
        // ignore content attributes
        if (core_link.getTarget() != "/" && !core_link.getTarget().empty()) {
          auto object_instance = makeObjectInstancePair(core_link);
          addToMap(result, object_instance);
        }
      }
      break;
    }
    default: {
      string error_msg =
          "Registration request must use " +
          toString(CoAP::ContentFormatType::CORE_LINK) + " fortmat type, not " +
          payload->getContentFormatType().getAsString() + " fortmat type.";
      throw domain_error(move(error_msg));
    }
    }
  }
  return result;
}

CoAP_RegistrationMessageDecoder::CoAP_RegistrationMessageDecoder(
    shared_ptr<DeviceRegistry> registration,
    shared_ptr<ThreadsafeHashSet<CoAP::Message>> message_buffer)
    : registration_(registration), message_buffer_(message_buffer),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

unique_ptr<ClientRequest_Register>
CoAP_RegistrationMessageDecoder::makeRegisterMessage(
    shared_ptr<CoAP::Message> input) {
  unique_ptr<ClientRequest_Register> result;
  for (auto option : input->getOptions()) {
    if (option->getOptionNumber() == OptionNumber::URI_PATH) {
      if (option->getAsString() == "rd") {
        string endpoint_name_;
        size_t life_time_ = 0;
        LwM2M_Version version_ = LwM2M_Version::UNRECOGNIZED;
        BindingType binding_ = BindingType::MALFORMED;
        bool queue_mode_ = false;
        string sms_number_;
        unordered_map<unsigned int, vector<unsigned int>>
            object_instances_map_ = getObjectList(input->getBody());
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

        result = make_unique<ClientRequest_Register>(
            input->getReceiverIP(), input->getReceiverPort(),
            input->getHeader().getMessageID(), input->getToken(),
            endpoint_name_, life_time_, version_, binding_, queue_mode_,
            sms_number_, object_instances_map_);
      }
    }
  }
  return result;
}

unique_ptr<ClientRequest_Deregister>
CoAP_RegistrationMessageDecoder::makeDeRegisterMessage(
    shared_ptr<CoAP::Message> input) {
  return make_unique<ClientRequest_Deregister>(
      input->getReceiverIP(), input->getReceiverPort(),
      input->getHeader().getMessageID(), input->getToken(),
      input->getOptions().at(1)->getAsString());
}

unique_ptr<ClientRequest_Update>
CoAP_RegistrationMessageDecoder::makeUpdateMessage(
    shared_ptr<CoAP::Message> input) {
  auto options = input->getOptions();
  string location = options.at(1)->getAsString();
  optional<size_t> life_time_ = nullopt;
  optional<BindingType> binding_ = nullopt;
  optional<string> sms_number_ = nullopt;
  auto object_instances_map = getObjectList(input->getBody());
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
  return make_unique<ClientRequest_Update>(
      input->getReceiverIP(), input->getReceiverPort(),
      input->getHeader().getMessageID(), input->getToken(), location,
      life_time_, binding_, sms_number_, object_instances_map);
}

bool CoAP_RegistrationMessageDecoder::tryDecode(
    shared_ptr<CoAP::Message> message) {
  if (message) {
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
  }
  return false;
}

void CoAP_RegistrationMessageDecoder::run() {
  try {
    while (!stopRequested()) {
      auto msg = message_buffer_->front();
      if (tryDecode(msg)) {
        message_buffer_->pop(msg);
      }
    }
  } catch (exception &ex) {
    logger_->log(HaSLL::SeverityLevel::ERROR, "Caught an exception: {}",
                 ex.what());
  }
}
} // namespace LwM2M