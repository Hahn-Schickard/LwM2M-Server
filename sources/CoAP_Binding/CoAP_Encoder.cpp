#include "CoAP_Encoder.hpp"
#include "CoAP/OptionBuilder.hpp"
#include "CoAP/PlainText.hpp"
#include "CoAP_ContentTypes.hpp"
#include "LoggerRepository.hpp"

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

HeaderPtr makeConfirmableHeader(CodeType code) {
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
  // @TODO: decide if this is needed at aöö
  return encode(static_pointer_cast<ServerRequest>(message));
}

CoAP::MessagePtr CoAP_Encoder::encode(InformationReportingRequestPtr message) {
  // @TODO: implement information reporting request encoding
  return CoAP::MessagePtr();
}

CoAP::MessagePtr CoAP_Encoder::encode(ServerRequestPtr request) {
  auto header = makeConfirmableHeader(toCodeType(request->message_type_));

  auto message = make_shared<CoAP::Message>(
      request->endpoint_->endpoint_address_, request->endpoint_->endpoint_port_,
      move(header));

  message->generateToken();

  auto options = makeOptions(request->message_type_, request->payload_);

  if (!options.empty()) {
    *message += options;
  }

  auto payload = encode(request->message_type_, request->payload_);
  if (payload) {
    *message += payload;
  }

  return message;
}

CoAP::MessagePtr CoAP_Encoder::encode(CoAP::MessagePtr request,
                                      ServerResponsePtr response) {
  if (response) {
    try {
      auto header = make_shared<CoAP::Header>(
          CoAP::MessageType::ACKNOWLEDGMENT,
          static_cast<TokenSize>(request->getToken().size()),
          toCodeType(response->response_code_),
          request->getHeader()->getMessageID());
      auto message = make_shared<CoAP::Message>(
          request->getAddressIP(), request->getAddressPort(), move(header));

      if (!request->getToken().empty()) {
        *message += request->getToken();
      }

      auto options = makeOptions(response->message_type_, response->payload_);
      if (!options.empty()) {
        *message += options;
      }

      if (response->payload_) {
        auto payload = encode(response->message_type_, response->payload_);
        if (payload) {
          *message += payload;
        }
      }
      return message;
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR,
                   "Caught an unhandled exception while encoding {} for CoAP "
                   "request {} from {}:{}. Exception: {}",
                   response->name(), request->getTokenAsHexString(),
                   request->getAddressIP(),
                   to_string(request->getAddressPort()), ex.what());
    }
  } else {
    return CoAP::MessagePtr();
  }
}

CoAP::PayloadPtr CoAP_Encoder::encode(LwM2M::MessageType type,
                                      LwM2M::PayloadPtr payload) {
  switch (type) {
  case LwM2M::MessageType::WRITE: {
    return encodeAs_TLV(payload);
  }
  case LwM2M::MessageType::WRITE_COMPOSITE: {
    return encodeAs_LwM2M_CBOR(payload);
  }
  case LwM2M::MessageType::EXECUTE: {
    return encodeAs_PlainText(payload);
  }
  default: { return CoAP::PayloadPtr(); }
  }
}

CoAP::Options CoAP_Encoder::makeOptions(LwM2M::MessageType type,
                                        LwM2M::PayloadPtr payload) {
  Options options;

  options = makeOptions(payload);

  switch (type) {
  case LwM2M::MessageType::READ: {
    auto acceptable_format_index = ContentFormatEncodings::LwM2M_TLV::index;
    options += build(OptionNumber::ACCEPT, acceptable_format_index);
    break;
  }
  case LwM2M::MessageType::READ_COMPOSITE: {
    auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    options += build(OptionNumber::CONTENT_FORMAT, content_format_index);
    break;
  }
  case LwM2M::MessageType::WRITE: {
    auto content_format_index = ContentFormatEncodings::LwM2M_TLV::index;
    options += build(OptionNumber::CONTENT_FORMAT, content_format_index);
    break;
  }
  case LwM2M::MessageType::WRITE_COMPOSITE: {
    auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    options += build(OptionNumber::CONTENT_FORMAT, content_format_index);
    break;
  }
  case LwM2M::MessageType::EXECUTE: {
    // check if there is some arguments first!
    auto content_format_index = ContentFormatEncodings::PlainText::index;
    options += build(OptionNumber::CONTENT_FORMAT, content_format_index);
    break;
  }
  case LwM2M::MessageType::CREATE: {
    auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    options += build(OptionNumber::CONTENT_FORMAT, content_format_index);
    break;
  }
  case LwM2M::MessageType::DISCOVER: {
    auto content_format_index = ContentFormatEncodings::CoRE_Link::index;
    options += build(OptionNumber::ACCEPT, content_format_index);
    break;
  }
  case LwM2M::MessageType::OBSERVE_COMPOSITE: {
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
    options += build(OptionNumber::OBSERVE, to_string(false));
    break;
  }
  default: { break; }
  }

  return options;
}

CoAP::Options CoAP_Encoder::makeOptions(LwM2M::PayloadPtr payload) {
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
