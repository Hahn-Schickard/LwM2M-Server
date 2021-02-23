#include "CoAP_Binding.hpp"
#include "CoAP/OptionBuilder.hpp"
#include "CoAP/PlainText.hpp"
#include "CoAP_ContentTypes.hpp"
#include "CoAP_RequestsManager.hpp"
#include "RegistrationInterfaceRequestsBuilder.hpp"
#include "Variant_Visitor.hpp"

using namespace std;
using namespace CoAP;

namespace LwM2M {

CoAP_Binding::CoAP_Binding(DeviceRegistryPtr registry,
                           CoAP_BindingConfigPtr config) {
  if (config) {
    match(config->address_,
          [&](bool ipv6_flag) {
            socket_ = make_shared<Socket>(ipv6_flag, config->port_);
          },
          [&](string address) {
            socket_ = make_shared<Socket>(address, config->port_);
          });
  } else {
    socket_ = make_shared<Socket>();
  }

  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_TLV>();
  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_CBOR>();
  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_JSON>();

  inbox_ = socket_->getInbox();

  BindingInterface::bind(make_shared<CoAP_RequestsManager>(
                             BindingInterface::response_handler_, socket_),
                         registry);
}

void CoAP_Binding::start() {
  socket_->start();
  Stoppable::start();
}

CoAP::MessagePtr CoAP_Binding::handleResponse(CoAP::MessagePtr message) {
  // get message id and token hash combo
  // create LwM2M::ClientResponse
  // call LwM2M::ResponseHandler::respond()
  return CoAP::MessagePtr();
}

CoAP::MessagePtr CoAP_Binding::handleNotification(CoAP::MessagePtr message) {
  // create LwM2M::ValueUpdated
  // forward to LwM2M::Notifier
  return CoAP::MessagePtr();
}

ServerResponsePtr
CoAP_Binding::handleRegistrationRequest(CoAP::MessagePtr message) {
  auto options = message->getOptions();
  auto option = options.find(CoAP::OptionNumber::URI_PATH);
  if (option != options.end()) {
    if (option->second->getAsString() == "rd") {
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
      } catch (ParameterNotFound &ex) {
        auto endpoint = make_shared<Endpoint>(message->getAddressIP(),
                                              message->getAddressPort());
        return ex.response_;
      }
    }
  }
  return ServerResponsePtr();
}

ServerResponsePtr CoAP_Binding::handleRequest(CoAP::MessagePtr message) {
  auto response = handleRegistrationRequest(message);
  if (response) {
    return move(response);
  }
  return ServerResponsePtr();
}

CoAP::CodeType toCodeType(ResponseCode code) {
  // Possible to do with reinterpert cast, but REALLY dangerous. This is a bit
  // more tedious, but 100% safe
  switch (code) {
  case ResponseCode::OK: {
    return CoAP::CodeType::OK;
  }
  case ResponseCode::CREATED: {
    return CoAP::CodeType::CREATED;
  }
  case ResponseCode::DELETED: {
    return CoAP::CodeType::DELETED;
  }
  case ResponseCode::CHANGED: {
    return CoAP::CodeType::CHANGED;
  }
  case ResponseCode::CONTENT: {
    return CoAP::CodeType::CONTENT;
  }
  case ResponseCode::CONTINUE: {
    return CoAP::CodeType::CONTINUE;
  }
  case ResponseCode::BAD_REQUEST: {
    return CoAP::CodeType::BAD_REQUEST;
  }
  case ResponseCode::UNAUTHORIZED: {
    return CoAP::CodeType::UNAUTHORIZED;
  }
  case ResponseCode::FORBIDDEN: {
    return CoAP::CodeType::FORBIDDEN;
  }
  case ResponseCode::NOT_FOUND: {
    return CoAP::CodeType::NOT_FOUND;
  }
  case ResponseCode::METHOD_NOT_ALLOWED: {
    return CoAP::CodeType::METHOD_NOT_ALLOWED;
  }
  case ResponseCode::NOT_ACCEPTABLE: {
    return CoAP::CodeType::NOT_ACCEPTABLE;
  }
  case ResponseCode::REQUEST_ENTITY_INCOMPLETE: {
    return CoAP::CodeType::REQUEST_ENTITY_INCOMPLETE;
  }
  case ResponseCode::PRECOGNITION_FAILED: {
    return CoAP::CodeType::PRECOGNITION_FAILED;
  }
  case ResponseCode::REQUEST_ENTITY_TOO_LARGE: {
    return CoAP::CodeType::REQUEST_ENTITY_TOO_LARGE;
  }
  case ResponseCode::UNSUPPORTED_CONTENT_FORMAT: {
    return CoAP::CodeType::UNSUPPORTED_CONTENT_FORMAT;
  }
  default:
    return CoAP::CodeType::UNHANDLED;
  }
}

CoAP::PayloadPtr buildPayload(ServerResponsePtr message) {
  if (message->interface_ == InterfaceType::REGISTRATION &&
      message->message_type_ == MessageType::REGISTER) {
  }
  return CoAP::PayloadPtr();
}

Options buildOptions(ServerResponsePtr message) {
  Options options;
  if (message->interface_ == InterfaceType::REGISTRATION &&
      message->message_type_ == MessageType::REGISTER) {
    if (holds_alternative<DataFormatPtr>(message->payload_->data_)) {
      auto data = std::get<DataFormatPtr>(message->payload_->data_);
      auto location = data->get<string>();
      auto rd_path = build(OptionNumber::LOCATION_PATH, "rd");
      auto location_path = build(OptionNumber::LOCATION_PATH, location);
      options.emplace(OptionNumber::LOCATION_PATH, move(rd_path));
      options.emplace(OptionNumber::LOCATION_PATH, move(location_path));
    }
  }

  return options;
}

CoAP::MessagePtr encode(CoAP::MessagePtr request, ServerResponsePtr message) {
  if (message) {
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
  } else {
    return CoAP::MessagePtr();
  }
}

CoAP::MessagePtr CoAP_Binding::handleMessage(CoAP::MessagePtr message) {
  if (message->getHeader()->getMesageType() ==
      CoAP::MessageType::ACKNOWLEDGMENT) {
    return handleResponse(message);
  } else if (message->getHeader()->getMesageType() ==
             CoAP::MessageType::NON_CONFIRMABLE) {
    return handleNotification(message);
  } else if (message->getHeader()->getMesageType() ==
             CoAP::MessageType::CONFIRMABLE) {
    return encode(message, handleRequest(message));
  } else {
    return CoAP::MessagePtr();
  }
}

void CoAP_Binding::run() {
  while (!stopRequested()) {
    auto message = inbox_->front();
    if (message) {
      auto response = handleMessage(message);
      if (response) {
        auto sent = socket_->send(response);
        sent.get();
      }
      inbox_->pop(message);
    }
  }
}

} // namespace LwM2M
