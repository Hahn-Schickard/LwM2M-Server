#include "CoAP_Binding.hpp"
#include "CoAP/CoRE_Link.hpp"
#include "CoAP/OctetStream.hpp"
#include "CoAP/OptionBuilder.hpp"
#include "CoAP/PlainText.hpp"
#include "CoAP_ContentTypes.hpp"
#include "CoAP_RequestsManager.hpp"
#include "RegistrationInterfaceRequestsBuilder.hpp"
#include "Utility.hpp"
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

ResponseCode toCodeType(CoAP::CodeType code) {
  // I will suffer, only if I f*ed up the CoAP::Socket implementation
  return static_cast<ResponseCode>(code);
}

PayloadPtr toPayload(PlainText content) {
  return make_shared<Payload>(make_shared<DataFormat>(content.toString()));
}

PayloadPtr toPayload(CoRE_Links content) {
  // TODO: handle CoRE Links for no nregistration purposes
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

ClientResponsePtr makeClientResponse(CoAP::MessagePtr message) {
  auto endpoint =
      make_shared<Endpoint>(message->getAddressIP(), message->getAddressPort());
  auto code = toCodeType(message->getHeader()->getCodeType());
  if (!message->getPayload()) {
    PayloadPtr content;
    auto payload = message->getPayload();
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
        throw runtime_error("Unhandeled Content Format with index :" +
                            content_format->getIndex());
      }
      }
      return make_shared<ClientResponse>(endpoint, code, content);
    }
  } else {
    return make_shared<ClientResponse>(endpoint, code);
  }
}

CoAP::MessagePtr CoAP_Binding::handleResponse(CoAP::MessagePtr message) {
  auto identifier = generateHash(message);
  if (response_handler_->exists(identifier)) {
    response_handler_->respond(identifier, makeClientResponse(message));
  }
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
