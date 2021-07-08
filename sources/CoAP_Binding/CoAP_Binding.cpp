#include "CoAP_Binding.hpp"
#include "CoAP/CoRE_Link.hpp"
#include "CoAP/OctetStream.hpp"
#include "CoAP/OptionBuilder.hpp"
#include "CoAP/PlainText.hpp"
#include "CoAP_ContentTypes.hpp"
#include "CoAP_RequestsManager.hpp"
#include "LoggerRepository.hpp"
#include "RegistrationInterfaceRequestsBuilder.hpp"
#include "Utility.hpp"

using namespace std;
using namespace CoAP;
using namespace HaSLL;

namespace LwM2M {

CoAP_Binding::CoAP_Binding(DeviceRegistryPtr registry,
                           const CoAP_Config &config)
    : Socket(config.address_, config.port_),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {

  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_TLV>();
  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_CBOR>();
  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_JSON>();

  BindingInterface::bind(
      make_shared<CoAP_RequestsManager>(
          BindingInterface::response_handler_,
          std::bind(&Socket::request, this, placeholders::_1)),
      registry);
}

CoAP_Binding::~CoAP_Binding() {
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

ResponseCode toCodeType(CoAP::CodeType code) {
  // I will suffer, only if I f*ed up the CoAP::Socket implementation
  return static_cast<ResponseCode>(code);
}

PayloadPtr toPayload(PlainText content) {
  return make_shared<Payload>(make_shared<DataFormat>(content.toString()));
}

PayloadPtr toPayload(CoRE_Links content) {
  // TODO: handle CoRE Links for non registration purposes
  return PayloadPtr();
}

PayloadPtr toPayload(TLV_Pack content) {
  auto values = content.getPackAsVector();
  if (!values.empty()) {
    if (values.size() == 1) {
      auto data = make_shared<DataFormat>(values[0]->getValue());
      return make_shared<Payload>(data);
    }
  }
  return PayloadPtr();
}

PayloadPtr toPayload(OctetStream content) {
  return make_shared<Payload>(make_shared<DataFormat>(content.getValue()));
}

ClientResponsePtr CoAP_Binding::makeClientResponse(CoAP::MessagePtr message) {
  auto endpoint =
      make_shared<Endpoint>(message->getAddressIP(), message->getAddressPort());
  try {
    auto code = toCodeType(message->getHeader()->getCodeType());
    if (auto payload = message->getPayload()) {
      PayloadPtr content;
      auto options = message->getOptions();
      auto it = options.find(OptionNumber::CONTENT_FORMAT);
      if (it != options.end()) {
        auto content_format =
            dynamic_pointer_cast<CoAP::ContentFormat>(it->second);
        switch (content_format->getIndex()) {
        case ContentFormatEncodings::PlainText::index: {
          auto text = decode<PlainText>(payload);
          content = toPayload(text);
          break;
        }
        case ContentFormatEncodings::CoRE_Link::index: {
          auto core_links = decode<CoRE_Links>(payload);
          content = toPayload(core_links);
          break;
        }
        case ContentFormatEncodings::LwM2M_TLV::index: {
          auto tlv = decode<TLV_Pack>(payload);
          content = toPayload(tlv);
          break;
        }
        case ContentFormatEncodings::LwM2M_CBOR::index: {
          [[fallthrough]];
        }
        case ContentFormatEncodings::LwM2M_JSON::index: {
          throw runtime_error(
              string(ContentFormatEncodings::LwM2M_JSON::toString()) +
              " is not supported");
        }
        case ContentFormatEncodings::Octet_Stream::index: {
          auto octets = decode<OctetStream>(payload);
          content = toPayload(octets);
          break;
        }
        default: {
          throw runtime_error("Unhandled Content Format with index :" +
                              content_format->getIndex());
        }
        }
        return make_shared<ClientResponse>(endpoint, code, content);
      }
    }
    return make_shared<ClientResponse>(endpoint, code);
  } catch (exception &ex) {
    logger_->log(SeverityLevel::CRITICAL,
                 "Caught an unhandled exception, while building a "
                 "ClientResponse from message {} from {}:{}. Exception: {}",
                 message->getTokenHash(), message->getAddressIP(),
                 message->getAddressPort(), ex.what());
    return make_shared<ClientResponse>(endpoint, ResponseCode::BAD_REQUEST);
  }
}

void CoAP_Binding::handleResponse(CoAP::MessagePtr message) {
  logger_->log(SeverityLevel::TRACE,
               "Handling incoming message from {}:{} as a Response.",
               message->getAddressIP(), message->getAddressPort());
  auto identifier = generateHash(message);
  if (response_handler_->exists(identifier)) {
    response_handler_->respond(identifier, makeClientResponse(message));
  } else {
    logger_->log(SeverityLevel::WARNNING,
                 "No Requests is Associated with {} response from {}:{}",
                 message->getTokenHash(), message->getAddressIP(),
                 message->getAddressPort());
  }
}

void CoAP_Binding::handleNotification(CoAP::MessagePtr message) {
  logger_->log(SeverityLevel::CRITICAL,
               "Notifications are not handeled by the server!");
  // create LwM2M::ValueUpdated
  // forward to LwM2M::Notifier
}

ServerResponsePtr
CoAP_Binding::handleRegistrationRequest(CoAP::MessagePtr message) {
  auto options = message->getOptions();
  auto option = options.find(CoAP::OptionNumber::URI_PATH);
  if (option != options.end()) {
    if (option->second->getAsString() == "rd") {
      logger_->log(SeverityLevel::TRACE,
                   "Handling incoming message from {}:{} as a Registration "
                   "Interface message.",
                   message->getAddressIP(), message->getAddressPort());
      try {
        if (message->getHeader()->getCodeType() == CoAP::CodeType::POST) {
          if (options.count(CoAP::OptionNumber::URI_PATH) > 1) {
            auto request = buildUpdateRequest(message);
            return registrator_->handleRquest(move(request));
          } else {
            auto request = buildRegisterRequest(message);
            return registrator_->handleRquest(move(request));
          }
        } else if (message->getHeader()->getCodeType() ==
                   CoAP::CodeType::DELETE) {
          auto request = buildDeregisterRequest(message);
          return registrator_->handleRquest(move(request));
        }
      } catch (RegistrationInterfaceError &ex) {
        auto endpoint = make_shared<Endpoint>(message->getAddressIP(),
                                              message->getAddressPort());
        return ex.response_;
      }
    }
  }
  return ServerResponsePtr();
}

ServerResponsePtr CoAP_Binding::handleRequest(CoAP::MessagePtr message) {
  logger_->log(SeverityLevel::TRACE,
               "Handling incoming message from {}:{} as a Request",
               message->getAddressIP(), message->getAddressPort());
  auto response = handleRegistrationRequest(message);
  if (response) {
    return move(response);
  }
  return ServerResponsePtr();
}

CoAP::CodeType toCodeType(ResponseCode code) {
  // Sometimes, you just have to live dangerously.
  return static_cast<CoAP::CodeType>(code);
}

CoAP::PayloadPtr buildPayload(ServerResponsePtr message) {
  if (message->interface_ == InterfaceType::REGISTRATION &&
      (message->message_type_ == MessageType::REGISTER ||
       message->message_type_ == MessageType::UPDATE)) {
  }
  return CoAP::PayloadPtr();
}

Options buildOptions(ServerResponsePtr message) {
  Options options;
  if (message->interface_ == InterfaceType::REGISTRATION &&
      (message->message_type_ == MessageType::REGISTER ||
       message->message_type_ == MessageType::UPDATE)) {
    if (message->payload_) {
      if (holds_alternative<DataFormatPtr>(message->payload_->data_)) {
        auto data = std::get<DataFormatPtr>(message->payload_->data_);
        if (data) {
          auto location = data->get<string>();
          auto rd_path = build(OptionNumber::LOCATION_PATH, "rd");
          auto location_path = build(OptionNumber::LOCATION_PATH, location);
          options.emplace(OptionNumber::LOCATION_PATH, move(rd_path));
          options.emplace(OptionNumber::LOCATION_PATH, move(location_path));
        } else {
          throw logic_error("LOCATION_PATH option value can not be an empty");
        }
      } else {
        throw logic_error(
            "LOCATION_PATH option value must be of DataFormatPtr type");
      }
    }
  }
  return options;
}

CoAP::MessagePtr CoAP_Binding::encode(CoAP::MessagePtr request,
                                      ServerResponsePtr message) {
  if (message) {
    try {
      auto header = make_shared<CoAP::Header>(
          CoAP::MessageType::ACKNOWLEDGMENT, request->getToken().size(),
          toCodeType(message->response_code_),
          request->getHeader()->getMessageID());
      auto response = make_shared<CoAP::Message>(
          request->getAddressIP(), request->getAddressPort(), move(header));

      if (!request->getToken().empty()) {
        *response += request->getToken();
      }

      auto options = buildOptions(message);
      if (!options.empty()) {
        *response += options;
      }

      if (message->payload_) {
        auto payload = buildPayload(message);
        if (payload) {
          *response += payload;
        }
      }
      return response;
    } catch (exception &ex) {
      logger_->log(SeverityLevel::CRITICAL,
                   "Caught an unhandled exception while encoding {} for "
                   "request {} form {}:{}. Exception: {}",
                   message->name(), request->getTokenHash(),
                   request->getAddressIP(), request->getAddressPort(),
                   ex.what());
      return CoAP::MessagePtr();
    }
  } else {
    return CoAP::MessagePtr();
  }
}

void CoAP_Binding::handleReceived(CoAP::MessagePtr message) {
  logger_->log(SeverityLevel::INFO, "Handling incoming message from {}:{}",
               message->getAddressIP(), message->getAddressPort());
  if (message->getHeader()->getMesageType() ==
      CoAP::MessageType::ACKNOWLEDGMENT) {
    handleResponse(message);
  } else if (message->getHeader()->getMesageType() ==
             CoAP::MessageType::NON_CONFIRMABLE) {
    handleNotification(message);
  } else if (message->getHeader()->getMesageType() ==
             CoAP::MessageType::CONFIRMABLE) {
    auto response = encode(message, handleRequest(message));
    respond(response);
  }
}

void CoAP_Binding::start() {
  Socket::start();
  logger_->log(SeverityLevel::INFO, "CoAP Binding started!");
}

void CoAP_Binding::stop() {
  Socket::stop();
  logger_->log(SeverityLevel::INFO, "CoAP Binding stoped!");
}
} // namespace LwM2M
