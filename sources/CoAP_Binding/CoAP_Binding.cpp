#include "CoAP_Binding.hpp"
#include "LoggerRepository.hpp"

#include "CoAP/CoRE_Link.hpp"
#include "CoAP/OctetStream.hpp"
#include "CoAP/OptionBuilder.hpp"
#include "CoAP/PlainText.hpp"
#include "CoAP_ContentTypes.hpp"
#include "Message.hpp"
#include "RegistrationInterfaceRequestsBuilder.hpp"
#include "TLV.hpp"
#include "Utility.hpp"

using namespace LwM2M;
using namespace std;
using namespace CoAP;
using namespace HaSLL;

#define ERROR_CODES_VALUE 0x80

ResponseCode toResponseCode(CoAP::CodeType code) {
  // I will suffer, only if I f*ed up the CoAP::Socket implementation
  return static_cast<ResponseCode>(code);
}

CodeType toCodeType(LwM2M::MessageType type) {
  switch (type) {
  case LwM2M::MessageType::OBSERVE:
  case LwM2M::MessageType::CANCEL_OBSERVATION:
  case LwM2M::MessageType::DISCOVER:
  case LwM2M::MessageType::READ: {
    return CodeType::GET;
  }
  case LwM2M::MessageType::OBSERVE_COMPOSITE:
  case LwM2M::MessageType::CANCEL_OBSERVATION_COMPOSITE:
  case LwM2M::MessageType::READ_COMPOSITE: {
    return CodeType::FETCH;
  }
  case LwM2M::MessageType::WRITE:
  case LwM2M::MessageType::WRITE_ATTRIBUTES: {
    return CodeType::PUT;
  }
  case LwM2M::MessageType::WRITE_COMPOSITE: {
    return CodeType::iPATCH;
  }
  case LwM2M::MessageType::CREATE:
  case LwM2M::MessageType::EXECUTE: {
    return CodeType::POST;
  }
  case LwM2M::MessageType::DELETE: {
    return CodeType::DELETE;
  }
  default: { throw logic_error("Message is not a valid ServerRequest."); }
  }
}

CoAP::CodeType toCodeType(ResponseCode code) {
  // Sometimes, you just have to live dangerously.
  return static_cast<CoAP::CodeType>(code);
}

HeaderPtr makeHeader(CodeType code) {
  return make_shared<Header>(CoAP::MessageType::CONFIRMABLE,
                             CoAP::TokenSize::_8, code);
}

CoAP::Options makeURI_PATH(ElementID target) {
  Options options;
  auto targets = target.toStrings();
  for (auto uri_path : targets) {
    options += build(OptionNumber::URI_PATH, uri_path);
  }
  return options;
}

CoAP::Options toOptions(LwM2M::PayloadPtr payload) {
  Options options;

  if (payload) {
    auto data = payload->data_;
    if (holds_alternative<TargetContent>(data)) {
      auto target_content = get<TargetContent>(data);
      options = makeURI_PATH(target_content.first);
    } else if (holds_alternative<TargetContentVector>(data)) {
      auto target_contents = get<TargetContentVector>(data);
      for (auto target_content : target_contents) {
        auto uri_path = makeURI_PATH(target_content.first);
        options.insert(uri_path.begin(), uri_path.end());
      }
    } else if (holds_alternative<ElementID>(data)) {
      auto target = get<ElementID>(data);
      options = makeURI_PATH(target);
    } else if (holds_alternative<ElementIDs>(data)) {
      auto targets = get<ElementIDs>(data);
      for (auto target : targets) {
        auto uri_path = makeURI_PATH(target);
        options.insert(uri_path.begin(), uri_path.end());
      }
    } else if (holds_alternative<vector<TargetAttribute>>(data)) {
      auto target_attributes = get<vector<TargetAttribute>>(data);
      for (auto target_attribute : target_attributes) {
        auto uri_path = makeURI_PATH(target_attribute.first);
        options.insert(uri_path.begin(), uri_path.end());
      }
    }
  }

  return options;
}

CoAP::Options makeOptions(ServerRequestPtr request) {
  Options options;

  options = toOptions(request->payload_);

  if (request->interface_ == InterfaceType::DEVICE_MANAGEMENT) {
    switch (request->message_type_) {
    case LwM2M::MessageType::READ: {
      auto acceptable_format_index = ContentFormatEncodings::LwM2M_TLV::index;
      options +=
          build(OptionNumber::ACCEPT, to_string(acceptable_format_index));
      break;
    }
    case LwM2M::MessageType::READ_COMPOSITE: {
      auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
      options +=
          build(OptionNumber::CONTENT_FORMAT, to_string(content_format_index));
      break;
    }
    case LwM2M::MessageType::WRITE: {
      auto content_format_index = ContentFormatEncodings::LwM2M_TLV::index;
      options +=
          build(OptionNumber::CONTENT_FORMAT, to_string(content_format_index));
      break;
    }
    case LwM2M::MessageType::WRITE_COMPOSITE: {
      auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
      options +=
          build(OptionNumber::CONTENT_FORMAT, to_string(content_format_index));
      break;
    }
    case LwM2M::MessageType::EXECUTE: {
      // check if there is some arguments first!
      auto content_format_index = ContentFormatEncodings::PlainText::index;
      options +=
          build(OptionNumber::CONTENT_FORMAT, to_string(content_format_index));
      break;
    }
    case LwM2M::MessageType::CREATE: {
      auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
      auto content_format =
          build(OptionNumber::CONTENT_FORMAT, to_string(content_format_index));
      options.emplace(OptionNumber::CONTENT_FORMAT, move(content_format));
      break;
    }
    case LwM2M::MessageType::DISCOVER: {
      auto content_format_index = ContentFormatEncodings::CoRE_Link::index;
      options += build(OptionNumber::ACCEPT, content_format_index);
    }
    default: { break; }
    }
  } else if (request->interface_ == InterfaceType::INFORMATION_REPORTING) {
    switch (request->message_type_) {
    case LwM2M::MessageType::OBSERVE_COMPOSITE: {
      auto content_format_index = ContentFormatEncodings::LwM2M_JSON::index;
      auto content_format =
          build(OptionNumber::CONTENT_FORMAT, to_string(content_format_index));
      options.emplace(OptionNumber::CONTENT_FORMAT, move(content_format));
      auto acceptable_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
      auto acceptable =
          build(OptionNumber::ACCEPT, to_string(acceptable_format_index));
      options.emplace(OptionNumber::ACCEPT, move(acceptable));
      [[fallthrough]];
    }
    case LwM2M::MessageType::OBSERVE: {
      options.emplace(OptionNumber::OBSERVE,
                      build(OptionNumber::OBSERVE, to_string(true)));
      break;
    }
    case LwM2M::MessageType::CANCEL_OBSERVATION:
    case LwM2M::MessageType::CANCEL_OBSERVATION_COMPOSITE: {
      options.emplace(OptionNumber::OBSERVE,
                      build(OptionNumber::OBSERVE, to_string(false)));
      break;
    }
    default: { break; }
    }
  }

  return options;
}

CoAP::PayloadPtr makeTLV(LwM2M::PayloadPtr payload) {
  if (payload) {
    auto data = payload->data_;
    if (holds_alternative<TargetContent>(data)) {
      // @TODO: build TLV_Pack here
      // auto tlv = makeTLV_pack(get<TargetContent>(data));
      // if (tlv) {
      //   return encode<TLV_Pack>(*tlv.get());
      // }
    }
  }
  return CoAP::PayloadPtr();
}

CoAP::PayloadPtr makeLwM2M_CBOR(LwM2M::PayloadPtr payload) {
  if (payload) {
    auto data = payload->data_;
    if (holds_alternative<vector<TargetContent>>(data)) {
      // encode in LwM2M_CBOR format here
    }
  }
  return CoAP::PayloadPtr();
}

CoAP::PayloadPtr makePlainText(LwM2M::PayloadPtr payload) {
  if (payload) {
    auto data = payload->data_;
    if (holds_alternative<DataFormatPtr>(data)) {
      auto arguments = get<DataFormatPtr>(data)->get<string>();
      return encode<PlainText>(arguments);
    }
  }
  return CoAP::PayloadPtr();
}

LwM2M::PayloadPtr toPayload(PlainText content) {
  return make_shared<LwM2M::Payload>(
      make_shared<DataFormat>(content.toString()));
}

/**
 * @brief Converts CoRE_Links content to std::vector<ElmentIdVariant> wrapped by
 * LwM2M::PayloadPtr
 *
 * @param content
 * @return LwM2M::PayloadPtr
 */
LwM2M::PayloadPtr toPayload(CoRE_Links content) {
  ElementIDs result;
  for (auto link : content.getLinks()) {
    auto targets = link.splitTarget('/');
    // focus on resource targets
    if (targets.size() == 3) {
      auto object_id = toUnsignedInteger(targets[0], IntegerBase::BASE_10);
      auto instance_id = toUnsignedInteger(targets[1], IntegerBase::BASE_10);
      auto resource_id = toUnsignedInteger(targets[2], IntegerBase::BASE_10);
      result.emplace_back(ElementID(object_id, instance_id, resource_id));
    }
  }
  if (!result.empty()) {
    return make_shared<LwM2M::Payload>(result);
  } else {
    return LwM2M::PayloadPtr();
  }
}

LwM2M::PayloadPtr toPayload(TLV_Pack content) {
  auto values = content.getPackAsVector();
  if (values.size() == 1) {
    if (values[0]->getIdentifierType() == Identifier_Type::Resource_Value) {
      auto data = make_shared<DataFormat>(values[0]->getValue());
      return make_shared<LwM2M::Payload>(data);
    } else if (values[0]->getIdentifierType() ==
               Identifier_Type::Object_Instance) {
      LwM2M::TargetContentVector targets_and_values;
      auto bytes = values[0]->getBytes();
      auto tlv_subpack = TLV_Pack(bytes);
      auto sub_values = tlv_subpack.getPackAsVector();
      for (auto sub_value : sub_values) {
        if (sub_value->getIdentifierType() == Identifier_Type::Resource_Value) {
          // Object id is in the request, so there is no way to get it from
          // response
          auto target = ElementID(0, values[0]->getIdentifier(),
                                  sub_value->getIdentifier());
          auto value = make_shared<DataFormat>(sub_value->getValue());
          targets_and_values.emplace_back(target, value);
        } else if (sub_value->getIdentifierType() ==
                   Identifier_Type::Multiple_Resources) {
          auto subpack_bytes = sub_value->getBytes();
          auto sub_sub_subpack = TLV_Pack(subpack_bytes);
          auto sub_sub_values = tlv_subpack.getPackAsVector();
          for (auto sub_sub_value : sub_sub_values) {
            // Same as before, Object ID is a mystery, without checkin the
            // original request
            auto target = ElementID(0, values[0]->getIdentifier(),
                                    sub_value->getIdentifier(),
                                    sub_sub_value->getIdentifier());
            auto value = make_shared<DataFormat>(sub_sub_value->getValue());
            targets_and_values.emplace_back(target, value);
          }
        }
        return make_shared<LwM2M::Payload>(targets_and_values);
      }
    }
  } else {
    return LwM2M::PayloadPtr();
  }
}

LwM2M::PayloadPtr toPayload(OctetStream content) {
  return make_shared<LwM2M::Payload>(
      make_shared<DataFormat>(content.getValue()));
}

CoAP::PayloadPtr buildPayload(ServerRequestPtr request) {
  switch (request->message_type_) {
  case LwM2M::MessageType::WRITE: {
    return makeTLV(request->payload_);
  }
  case LwM2M::MessageType::WRITE_COMPOSITE: {
    return makeLwM2M_CBOR(request->payload_);
  }
  case LwM2M::MessageType::EXECUTE: {
    return makePlainText(request->payload_);
  }
  default: { return CoAP::PayloadPtr(); }
  }
}

CoAP::PayloadPtr buildPayload(ServerResponsePtr message) {
  if (message->interface_ == InterfaceType::REGISTRATION &&
      (message->message_type_ == LwM2M::MessageType::REGISTER ||
       message->message_type_ == LwM2M::MessageType::UPDATE)) {
    // No Payload for registration interface responses
  }
  return CoAP::PayloadPtr();
}

Options buildOptions(ServerResponsePtr message) {
  Options options;
  if (message->interface_ == InterfaceType::REGISTRATION &&
      (message->message_type_ == LwM2M::MessageType::REGISTER ||
       message->message_type_ == LwM2M::MessageType::UPDATE)) {
    if (message->payload_) {
      if (holds_alternative<DataFormatPtr>(message->payload_->data_)) {
        auto data = std::get<DataFormatPtr>(message->payload_->data_);
        if (data) {
          options += build(OptionNumber::LOCATION_PATH, "rd");
          auto location = data->get<string>();
          options += build(OptionNumber::LOCATION_PATH, location);
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

CoAP::MessagePtr encode(CoAP::MessagePtr request, ServerResponsePtr message) {
  if (message) {
    try {
      auto header = make_shared<CoAP::Header>(
          CoAP::MessageType::ACKNOWLEDGMENT,
          static_cast<TokenSize>(request->getToken().size()),
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
      string error_msg =
          "Caught an unhandled exception while encoding " + message->name() +
          " for request " + request->getTokenAsHexString() + " form " +
          request->getAddressIP() + ":" + to_string(request->getAddressPort()) +
          " Exception: " + ex.what();
      throw runtime_error(error_msg);
    }
  } else {
    return CoAP::MessagePtr();
  }
}

CoAP::MessagePtr encode(ServerRequestPtr request) {
  auto header = makeHeader(toCodeType(request->message_type_));
  auto message = make_shared<CoAP::Message>(
      request->endpoint_->endpoint_address_, request->endpoint_->endpoint_port_,
      move(header));
  message->generateToken();
  auto options = makeOptions(request);

  if (!options.empty()) {
    *message += options;
  }
  auto payload = buildPayload(request);
  if (payload) {
    *message += payload;
  }

  return message;
}

CoAP_Binding::CoAP_Binding(DeviceRegistryPtr registry,
                           const string &config_filepath)
    : CoAP_Binding(registry, getConfig(config_filepath)) {}

CoAP_Binding::CoAP_Binding(DeviceRegistryPtr registry,
                           const CoAP::Configuration &config)
    : BindingInterface(registry), Registrator(registry), Socket(config),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  logger_->log(SeverityLevel::TRACE, "Registering CoAP Server");

  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_TLV>();
  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_CBOR>();
  SupportedContentFormats::addNewContentFormatType<
      ContentFormatEncodings::LwM2M_JSON>();
}

CoAP_Binding::~CoAP_Binding() {
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

void CoAP_Binding::start() {
  Socket::start();
  logger_->log(SeverityLevel::INFO, "CoAP Server started!");
}

void CoAP_Binding::stop() {
  Socket::stop();
  logger_->log(SeverityLevel::INFO, "CoAP Server stoped!");
}

ClientResponsePtr CoAP_Binding::makeClientResponse(CoAP::MessagePtr message) {
  auto endpoint =
      make_shared<Endpoint>(message->getAddressIP(), message->getAddressPort());
  try {
    auto code = toResponseCode(message->getHeader()->getCodeType());
    if (auto payload = message->getPayload()) {
      LwM2M::PayloadPtr content;
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

future<DataFormatPtr> CoAP_Binding::requestData(ServerRequestPtr message) {
  return async(
      launch::async,
      [this](ServerRequestPtr request) -> DataFormatPtr {
        auto coap_request = encodeRequest(request);
        logger_->log(SeverityLevel::TRACE,
                     "Dispatching {}:{} as a data request to {}",
                     request->name(), coap_request->getTokenHash(),
                     request->endpoint_->toString());
        auto response_future = CoAP::Socket::request(coap_request);
        auto coap_response = response_future.get();
        logger_->log(
            SeverityLevel::TRACE,
            "Received a response with token {} to data request {} from {}",
            coap_response->getTokenHash(), request->name(),
            request->endpoint_->toString());
        auto response = makeClientResponse(coap_response);
        logger_->log(SeverityLevel::TRACE, "Decoded {} as a {}",
                     coap_response->getTokenHash(), response->name());
        dispatched_.erase(std::hash<Message>{}(*request));
        if (response->response_code_ == ResponseCode::CONTENT) {
          if (response->payload_) {
            return std::get<DataFormatPtr>(response->payload_->data_);
          } else {
            throw ResponseReturnedAnEmptyPayload(response, request);
          }
        } else {
          throw ResponseReturnedAnErrorCode(response, request);
        }
      },
      message);
}

future<TargetContentVector>
CoAP_Binding::requestMultiTargetData(ServerRequestPtr message) {
  return async(
      launch::async,
      [this](ServerRequestPtr request) -> TargetContentVector {
        auto coap_request = encodeRequest(request);
        logger_->log(SeverityLevel::TRACE,
                     "Dispatching {}:{} as a multi target request to {}",
                     request->name(), coap_request->getTokenHash(),
                     request->endpoint_->toString());
        auto response_future = CoAP::Socket::request(coap_request);
        auto coap_response = response_future.get();
        logger_->log(SeverityLevel::TRACE,
                     "Received a response with token {} to multi target "
                     "request {} from {}",
                     coap_response->getTokenHash(), request->name(),
                     request->endpoint_->toString());
        auto response = makeClientResponse(coap_response);
        logger_->log(SeverityLevel::TRACE, "Decoded {} as a {}",
                     coap_response->getTokenHash(), response->name());
        dispatched_.erase(std::hash<Message>{}(*request));
        if (response->response_code_ == ResponseCode::CONTENT) {
          if (response->payload_) {
            return std::get<TargetContentVector>(response->payload_->data_);
          } else {
            throw ResponseReturnedAnEmptyPayload(response, request);
          }
        } else {
          throw ResponseReturnedAnErrorCode(response, request);
        }
      },
      message);
}

future<bool> CoAP_Binding::requestAction(ServerRequestPtr message) {
  return async(
      launch::async,
      [this](ServerRequestPtr request) -> bool {
        auto coap_request = encodeRequest(request);
        logger_->log(SeverityLevel::TRACE,
                     "Dispatching {}:{} as an action to {}", request->name(),
                     coap_request->getTokenHash(),
                     request->endpoint_->toString());
        auto response_future = CoAP::Socket::request(coap_request);
        auto coap_response = response_future.get();
        logger_->log(
            SeverityLevel::TRACE,
            "Received a response with token {} to an action {} from {}",
            coap_response->getTokenHash(), request->name(),
            request->endpoint_->toString());
        auto response = makeClientResponse(coap_response);
        logger_->log(SeverityLevel::TRACE, "Decoded {} as a {}",
                     coap_response->getTokenHash(), response->name());
        dispatched_.erase(std::hash<Message>{}(*request));
        return static_cast<uint8_t>(response->response_code_) <
                       ERROR_CODES_VALUE
                   ? true
                   : false;
      },
      message);
}

future<ClientResponsePtr> CoAP_Binding::request(ServerRequestPtr message) {
  return async(
      launch::async,
      [this](ServerRequestPtr request) -> ClientResponsePtr {
        auto coap_request = encodeRequest(request);
        logger_->log(SeverityLevel::TRACE,
                     "Dispatching {}:{} as a generic request to {}",
                     request->name(), coap_request->getTokenHash(),
                     request->endpoint_->toString());
        auto response_future = CoAP::Socket::request(coap_request);
        auto coap_response = response_future.get();
        logger_->log(
            SeverityLevel::TRACE,
            "Received a response with token {} to generic request {} from {}",
            coap_response->getTokenHash(), request->name(),
            request->endpoint_->toString());
        auto response = makeClientResponse(coap_response);
        logger_->log(SeverityLevel::TRACE, "Decoded {} as a {}",
                     coap_response->getTokenHash(), response->name());
        dispatched_.erase(std::hash<Message>{}(*request));
        return move(response);
      },
      message);
}

void CoAP_Binding::cancelRequest(ServerRequestPtr message) {
  auto request = encodeRequest(message);
  if (request) {
    logger_->log(SeverityLevel::TRACE, "Canceling {}:{} for {}",
                 message->name(), request->getTokenHash(),
                 message->endpoint_->toString());
    CoAP::Socket::cancelRequest(request);
    logger_->log(SeverityLevel::TRACE, "{}:{} for {} canceled!",
                 message->name(), request->getTokenHash(),
                 message->endpoint_->toString());
  } else {
    logger_->log(SeverityLevel::ERROR, "Can not cancel an empty request!");
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
            return Registrator::handleRquest(move(request));
          } else {
            auto request = buildRegisterRequest(message);
            return Registrator::handleRquest(move(request));
          }
        } else if (message->getHeader()->getCodeType() ==
                   CoAP::CodeType::DELETE) {
          auto request = buildDeregisterRequest(message);
          return Registrator::handleRquest(move(request));
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

void CoAP_Binding::handleReceived(CoAP::MessagePtr message) {
  logger_->log(SeverityLevel::INFO, "Handling incoming message from {}:{}",
               message->getAddressIP(), message->getAddressPort());
  if (message->getHeader()->getMesageType() ==
      CoAP::MessageType::ACKNOWLEDGMENT) {
    logger_->log(SeverityLevel::WARNNING,
                 "Received an orphan response from {}:{} with ID {}",
                 message->getAddressIP(), message->getAddressPort(),
                 message->getTokenAsHexString());
  } else if (message->getHeader()->getMesageType() ==
             CoAP::MessageType::NON_CONFIRMABLE) {
    handleNotification(message);
  } else if (message->getHeader()->getMesageType() ==
             CoAP::MessageType::CONFIRMABLE) {
    auto response = encode(message, handleRequest(message));
    respond(response);
  }
}

CoAP::MessagePtr CoAP_Binding::encodeRequest(ServerRequestPtr request) {
  auto hash = std::hash<Message>{}(*request);
  auto it = dispatched_.find(hash);
  if (it == dispatched_.end()) {
    auto msg = encode(request);
    dispatched_.emplace(hash, msg);
    return msg;
  } else {
    return it->second;
  }
}
