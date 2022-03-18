#include "CoAP_Encoder.hpp"
#include "CoAP_ContentTypes.hpp"
#include "LoggerRepository.hpp"

#include "CoAPS4Cpp/MessageBuilder.hpp"
#include "CoAPS4Cpp/OptionBuilder.hpp"
#include "CoAPS4Cpp/PlainText.hpp"

using namespace std;
using namespace CoAP;
using namespace HaSLL;

namespace LwM2M {
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

CoAP::Options makeURI_PATH(ElementID target) {
  Options options;
  auto targets = target.toStrings();
  for (auto uri_path : targets) {
    options += build(OptionNumber::URI_PATH, uri_path);
  }
  return options;
}
CoAP::PayloadPtr encodeAs_TLV(LwM2M::PayloadPtr payload) {
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

CoAP::PayloadPtr encodeAs_LwM2M_CBOR(LwM2M::PayloadPtr payload) {
  if (payload) {
    auto data = payload->data_;
    if (holds_alternative<vector<TargetContent>>(data)) {
      // encode in LwM2M_CBOR format here
    }
  }
  return CoAP::PayloadPtr();
}

CoAP::PayloadPtr encodeAs_PlainText(LwM2M::PayloadPtr payload) {
  if (payload) {
    auto data = payload->data_;
    if (holds_alternative<DataFormatPtr>(data)) {
      auto arguments = get<DataFormatPtr>(data)->get<string>();
      return encode<PlainText>(arguments);
    }
  }
  return CoAP::PayloadPtr();
}

CoAP_Encoder::CoAP_Encoder()
    : logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

CoAP_Encoder::~CoAP_Encoder() {
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

CoAP::MessagePtr CoAP_Encoder::encode(DeviceManagementRequestPtr message) {
  // @TODO: decide if this is needed at all
  return encode(static_pointer_cast<ServerRequest>(message));
}

CoAP::MessagePtr CoAP_Encoder::encode(InformationReportingRequestPtr message) {
  // @TODO: implement information reporting request encoding
  return encode(static_pointer_cast<ServerRequest>(message));
}

CoAP::MessagePtr CoAP_Encoder::encode(ServerRequestPtr request) {
  auto mesage_builder =
      make_unique<MessageBuilder>(request->endpoint_->endpoint_address_,
                                  request->endpoint_->endpoint_port_);
  auto token = mesage_builder->addToken();
  mesage_builder->addHeader(CoAP::MessageType::CONFIRMABLE,
                            toCodeType(request->message_type_));
  mesage_builder->addOptions(
      makeOptions(request->message_type_, request->payload_, token->hexify()));
  auto payload =
      encode(request->message_type_, request->payload_, token->hexify());
  if (payload) {
    mesage_builder->addPayload(payload);
  }
  return mesage_builder->getResult();
}

CoAP::MessagePtr CoAP_Encoder::encode(CoAP::MessagePtr request,
                                      ServerResponsePtr response) {
  auto mesage_builder = make_unique<MessageBuilder>(request->getAddressIP(),
                                                    request->getAddressPort());
  auto token = request->getToken();
  mesage_builder->addToken(token);
  if (response) {
    try {
      mesage_builder->addHeader(CoAP::MessageType::ACKNOWLEDGMENT,
                                toCodeType(response->response_code_),
                                request->getHeader()->getMessageID());
      mesage_builder->addOptions(makeOptions(
          response->message_type_, response->payload_, token->hexify()));
      if (auto payload = encode(response->message_type_, response->payload_,
                                token->hexify())) {
        mesage_builder->addPayload(payload);
      }
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR,
                   "Caught an unhandled exception while encoding {} for CoAP "
                   "request {} from {}:{}. Exception: {}",
                   response->name(), request->getToken()->hexify(),
                   request->getAddressIP(),
                   to_string(request->getAddressPort()), ex.what());
    }
  } else {
    // handle empty response or exception throw
    mesage_builder->addHeader(CoAP::MessageType::ACKNOWLEDGMENT,
                              CoAP::CodeType::BAD_REQUEST,
                              request->getHeader()->getMessageID());
  }
  return mesage_builder->getResult();
}

CoAP::PayloadPtr CoAP_Encoder::encode(LwM2M::MessageType type,
                                      LwM2M::PayloadPtr payload,
                                      string message_identifier) {
  switch (type) {
  case LwM2M::MessageType::WRITE: {
    logger_->log(SeverityLevel::TRACE,
                 "Encoding message {} payload as a LwM2M TLV",
                 message_identifier);
    return encodeAs_TLV(payload);
  }
  case LwM2M::MessageType::WRITE_COMPOSITE: {
    logger_->log(SeverityLevel::TRACE,
                 "Encoding message {} payload as a LwM2M CBOR",
                 message_identifier);
    return encodeAs_LwM2M_CBOR(payload);
  }
  case LwM2M::MessageType::EXECUTE: {
    logger_->log(SeverityLevel::TRACE,
                 "Encoding message {} payload as PlainText",
                 message_identifier);
    return encodeAs_PlainText(payload);
  }
  case LwM2M::MessageType::REGISTER: {
    [[fallthrough]];
  }
  case LwM2M::MessageType::UPDATE: {
    [[fallthrough]];
  }
  case LwM2M::MessageType::DEREGISTER: {
    [[fallthrough]];
  }
  case LwM2M::MessageType::DISCOVER: {
    logger_->log(SeverityLevel::TRACE,
                 "{} Message {} does not have a CoAP payload to encode.",
                 toString(type), message_identifier);
    return CoAP::PayloadPtr();
  }
  default: {
    logger_->log(SeverityLevel::WARNNING,
                 "Did not encode message {} payload for {}", message_identifier,
                 toString(type));
    return CoAP::PayloadPtr();
  }
  }
}

CoAP::Options CoAP_Encoder::makeOptions(LwM2M::MessageType type,
                                        LwM2M::PayloadPtr payload,
                                        string message_identifier) {
  Options options;

  switch (type) {
  case LwM2M::MessageType::REGISTER: {
    [[fallthrough]];
  }
  case LwM2M::MessageType::UPDATE: {
    if (payload) {
      if (holds_alternative<DataFormatPtr>(payload->data_)) {
        auto data = std::get<DataFormatPtr>(payload->data_);
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
    } else {
      string error_msg = toString(type) + " response must have a payload";
      throw logic_error(error_msg);
    }
    break;
  }
  case LwM2M::MessageType::READ: {
    options = makeOptions(payload, message_identifier);
    auto acceptable_format_index = ContentFormatEncodings::LwM2M_TLV::index;
    options += build(OptionNumber::ACCEPT, acceptable_format_index);
    break;
  }
  case LwM2M::MessageType::READ_COMPOSITE: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    options += build(OptionNumber::CONTENT_FORMAT, content_format_index);
    break;
  }
  case LwM2M::MessageType::WRITE: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::LwM2M_TLV::index;
    options += build(OptionNumber::CONTENT_FORMAT, content_format_index);
    break;
  }
  case LwM2M::MessageType::WRITE_COMPOSITE: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    options += build(OptionNumber::CONTENT_FORMAT, content_format_index);
    break;
  }
  case LwM2M::MessageType::EXECUTE: {
    // check if there is some arguments first!
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::PlainText::index;
    options += build(OptionNumber::CONTENT_FORMAT, content_format_index);
    break;
  }
  case LwM2M::MessageType::CREATE: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    options += build(OptionNumber::CONTENT_FORMAT, content_format_index);
    break;
  }
  case LwM2M::MessageType::DISCOVER: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::CoRE_Link::index;
    options += build(OptionNumber::ACCEPT, content_format_index);
    break;
  }
  case LwM2M::MessageType::OBSERVE_COMPOSITE: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::LwM2M_JSON::index;
    auto content_format =
        build(OptionNumber::CONTENT_FORMAT, to_string(content_format_index));
    options.emplace(OptionNumber::CONTENT_FORMAT, move(content_format));
    auto acceptable_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    options += build(OptionNumber::ACCEPT, acceptable_format_index);
    [[fallthrough]];
  }
  case LwM2M::MessageType::OBSERVE: {
    options += build(OptionNumber::OBSERVE, to_string(true));
    break;
  }
  case LwM2M::MessageType::CANCEL_OBSERVATION:
    [[fallthrough]];
  case LwM2M::MessageType::CANCEL_OBSERVATION_COMPOSITE: {
    options = makeOptions(payload, message_identifier);
    options += build(OptionNumber::OBSERVE, to_string(false));
    break;
  }
  default: { break; }
  }
  return options;
} // namespace LwM2M

CoAP::Options CoAP_Encoder::makeOptions(LwM2M::PayloadPtr payload,
                                        string message_identifier) {
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

} // namespace LwM2M
