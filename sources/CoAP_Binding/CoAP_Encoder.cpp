#include "CoAP_Encoder.hpp"
#include "CoAP_ContentTypes.hpp"

#include "CoAPS4Cpp/MessageBuilder.hpp"
#include "CoAPS4Cpp/OptionBuilder.hpp"
#include "CoAPS4Cpp/PlainText.hpp"
#include "HaSLL/LoggerManager.hpp"

using namespace std;
using namespace CoAP;
using namespace HaSLL;

namespace LwM2M {
CodeType toCodeType(LwM2M::MessageType type) {
  switch (type) {
  case LwM2M::MessageType::Observe:
    [[fallthrough]];
  case LwM2M::MessageType::Cancel_Observation:
    [[fallthrough]];
  case LwM2M::MessageType::Discover:
    [[fallthrough]];
  case LwM2M::MessageType::Read: {
    return CodeType::Get;
  }
  case LwM2M::MessageType::Observe_Composite:
  case LwM2M::MessageType::Cancel_Observation_Composite:
  case LwM2M::MessageType::Read_Composite: {
    return CodeType::Fetch;
  }
  case LwM2M::MessageType::Write:
  case LwM2M::MessageType::Write_Attributes: {
    return CodeType::Put;
  }
  case LwM2M::MessageType::Write_Composite: {
    return CodeType::iPatch;
  }
  case LwM2M::MessageType::Create:
  case LwM2M::MessageType::Execute: {
    return CodeType::Post;
  }
  case LwM2M::MessageType::Delete: {
    return CodeType::Delete;
  }
  default: {
    throw logic_error("Message is not a valid ServerRequest.");
  }
  }
}

CoAP::CodeType toCodeType(ResponseCode code) {
  // Sometimes, you just have to live dangerously.
  return static_cast<CoAP::CodeType>(code);
}

// NOLINTBEGIN(readability-identifier-naming)
CoAP::Options makeURI_PATH(ElementID target) {
  Options options;
  auto targets = target.toStrings();
  for (const auto& uri_path : targets) {
    options += build(OptionNumber::URI_Path, uri_path);
  }
  return options;
}

CoAP::PayloadPtr encodeAs_TLV(const LwM2M::PayloadPtr& payload) {
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

CoAP::PayloadPtr encodeAs_LwM2M_CBOR(const LwM2M::PayloadPtr& payload) {
  if (payload) {
    auto data = payload->data_;
    if (holds_alternative<vector<TargetContent>>(data)) {
      // encode in LwM2M_CBOR format here
    }
  }
  return CoAP::PayloadPtr();
}

CoAP::PayloadPtr encodeAs_PlainText(const LwM2M::PayloadPtr& payload) {
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
    : logger_(LoggerManager::registerTypedLogger(this)) {}

CoAP_Encoder::~CoAP_Encoder() {
  LoggerManager::deregisterLogger(logger_->getName());
}
// NOLINTEND(readability-identifier-naming)

CoAP::MessagePtr CoAP_Encoder::encode(
    const DeviceManagementRequestPtr& message) {
  // @TODO: decide if this is needed at all
  return encode(static_pointer_cast<ServerRequest>(message));
}

CoAP::MessagePtr CoAP_Encoder::encode(
    const InformationReportingRequestPtr& message) {
  // @TODO: decide if this is needed at all
  return encode(static_pointer_cast<ServerRequest>(message));
}

CoAP::MessagePtr CoAP_Encoder::encode(const ServerRequestPtr& request) {
  auto message_builder =
      make_unique<MessageBuilder>(request->endpoint_->endpoint_address_,
          request->endpoint_->endpoint_port_);
  auto token = message_builder->addToken();
  message_builder->addHeader(
      CoAP::MessageType::Confirmable, toCodeType(request->message_type_));
  message_builder->addOptions(
      makeOptions(request->message_type_, request->payload_, token->hexify()));
  auto payload =
      encode(request->message_type_, request->payload_, token->hexify());
  if (payload) {
    message_builder->addPayload(payload);
  }
  return message_builder->getResult();
}

CoAP::MessagePtr CoAP_Encoder::encode(
    const CoAP::MessagePtr& request, const ServerResponsePtr& response) {
  auto message_builder = make_unique<MessageBuilder>(
      request->getAddressIP(), request->getAddressPort());
  auto token = request->getToken();
  message_builder->addToken(token);
  if (response) {
    try {
      message_builder->addHeader(CoAP::MessageType::Acknowledgment,
          toCodeType(response->response_code_),
          request->getHeader()->getMessageID());
      message_builder->addOptions(makeOptions(
          response->message_type_, response->payload_, token->hexify()));
      if (auto payload = encode(
              response->message_type_, response->payload_, token->hexify())) {
        message_builder->addPayload(payload);
      }
    } catch (const exception& ex) {
      logger_->error("Caught an unhandled exception while encoding {} for CoAP "
                     "request {} from {}:{}. Exception: {}",
          response->name(), request->getToken()->hexify(),
          request->getAddressIP(), to_string(request->getAddressPort()),
          ex.what());
    }
  } else {
    // handle empty response or exception throw
    message_builder->addHeader(CoAP::MessageType::Acknowledgment,
        CoAP::CodeType::Bad_Request, request->getHeader()->getMessageID());
  }
  return message_builder->getResult();
}

CoAP::PayloadPtr CoAP_Encoder::encode(LwM2M::MessageType type,
    const LwM2M::PayloadPtr& payload, const string& message_identifier) {
  switch (type) {
  case LwM2M::MessageType::Write: {
    logger_->trace("Encoding {} message {} payload as a LwM2M TLV",
        toString(type), message_identifier);
    return encodeAs_TLV(payload);
  }
  case LwM2M::MessageType::Write_Composite: {
    logger_->trace("Encoding {} message {} payload as a LwM2M CBOR",
        toString(type), message_identifier);
    return encodeAs_LwM2M_CBOR(payload);
  }
  case LwM2M::MessageType::Execute: {
    logger_->trace("Encoding {} message {} payload as PlainText",
        toString(type), message_identifier);
    return encodeAs_PlainText(payload);
  }
  case LwM2M::MessageType::Observe_Composite: {
    [[fallthrough]];
  }
  case LwM2M::MessageType::Cancel_Observation_Composite: {
    logger_->trace("Encoding {} message {} payload as a LwM2M CBOR",
        toString(type), message_identifier);
    return encodeAs_LwM2M_CBOR(payload);
  }
  default: {
    return CoAP::PayloadPtr();
  }
  }
}

CoAP::Options CoAP_Encoder::makeOptions(LwM2M::MessageType type,
    const LwM2M::PayloadPtr& payload, const string& message_identifier) {
  Options options;

  switch (type) {
  case LwM2M::MessageType::Register: {
    [[fallthrough]];
  }
  case LwM2M::MessageType::Update: {
    if (payload) {
      if (holds_alternative<DataFormatPtr>(payload->data_)) {
        const auto& data = std::get<DataFormatPtr>(payload->data_);
        if (data) {
          options += build(OptionNumber::Location_Path, "rd");
          auto location = data->get<string>();
          logger_->trace(
              "Assiging Location Path CoAP::Option value as rd/{} for "
              "{} message {}",
              toString(type), location, message_identifier);
          options += build(OptionNumber::Location_Path, location);
        } else {
          throw logic_error("LOCATION_PATH option value can not be an empty");
        }
      } else {
        throw logic_error(
            "LOCATION_PATH option value must be of DataFormatPtr type");
      }
    }
    break;
  }
  case LwM2M::MessageType::Read: {
    options = makeOptions(payload, message_identifier);
    auto acceptable_format_index = ContentFormatEncodings::LwM2M_TLV::index;
    auto accept = build(OptionNumber::Accept, acceptable_format_index);
    options += accept;
    logger_->trace("Assiging Accept CoAP::Option value as {} for {} message {}",
        toString(type), accept->getValueAsString(), message_identifier);
    break;
  }
  case LwM2M::MessageType::Read_Composite: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    auto content_format =
        build(OptionNumber::Content_Format, content_format_index);
    options += content_format;
    logger_->trace(
        "Assiging Content Format CoAP::Option value as {} for {} message {}",
        toString(type), content_format->getValueAsString(), message_identifier);
    break;
  }
  case LwM2M::MessageType::Write: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::LwM2M_TLV::index;
    auto content_format =
        build(OptionNumber::Content_Format, content_format_index);
    options += content_format;
    logger_->trace(
        "Assiging Content Format CoAP::Option value as {} for {} message {}",
        toString(type), content_format->getValueAsString(), message_identifier);
    break;
  }
  case LwM2M::MessageType::Write_Composite: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    auto content_format =
        build(OptionNumber::Content_Format, content_format_index);
    options += content_format;
    logger_->trace(
        "Assiging Content Format CoAP::Option value as {} for {} message {}",
        toString(type), content_format->getValueAsString(), message_identifier);
    break;
  }
  case LwM2M::MessageType::Execute: {
    // @TODO: check if there is some arguments first!
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::PlainText::index;
    auto content_format =
        build(OptionNumber::Content_Format, content_format_index);
    options += content_format;
    logger_->trace(
        "Assiging Content Format CoAP::Option value as {} for {} message {}",
        toString(type), content_format->getValueAsString(), message_identifier);
    break;
  }
  case LwM2M::MessageType::Create: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    auto content_format =
        build(OptionNumber::Content_Format, content_format_index);
    options += content_format;
    logger_->trace(
        "Assiging Content Format CoAP::Option value as {} for {} message {}",
        toString(type), content_format->getValueAsString(), message_identifier);
    break;
  }
  case LwM2M::MessageType::Discover: {
    options = makeOptions(payload, message_identifier);
    auto content_format_index = ContentFormatEncodings::CoRE_Link::index;
    auto content_format = build(OptionNumber::Accept, content_format_index);
    options += content_format;
    logger_->trace(
        "Assiging Content Format CoAP::Option value as {} for {} message {}",
        toString(type), content_format->getValueAsString(), message_identifier);
    break;
  }
  case LwM2M::MessageType::Observe_Composite: {
    auto content_format_index = ContentFormatEncodings::LwM2M_JSON::index;
    auto json_content_format =
        build(OptionNumber::Content_Format, content_format_index);
    options += json_content_format;
    logger_->trace(
        "Assiging Content Format CoAP::Option value as {} for {} message {}",
        toString(type), json_content_format->getValueAsString(),
        message_identifier);
    auto acceptable_format_index = ContentFormatEncodings::LwM2M_CBOR::index;
    auto cbor_content_format =
        build(OptionNumber::Content_Format, acceptable_format_index);
    options += cbor_content_format;
    logger_->trace(
        "Assiging Content Format CoAP::Option value as {} for {} message {}",
        toString(type), cbor_content_format->getValueAsString(),
        message_identifier);
    options += build(OptionNumber::Observe, 0);
    logger_->trace("Assiging Observe CoAP::Option value as 0 for {} message {}",
        toString(type), message_identifier);
    // @Attention URI paths for resources to be observed MUST be provided in
    // request payload via CoAP::PayloadPtr
    // CoAP_Encoder::encode(LwM2M::MessageType,LwM2M::PayloadPtr,string) method
    break;
  }
  case LwM2M::MessageType::Observe: {
    options = makeOptions(payload, message_identifier);
    options += build(OptionNumber::Observe, 0);
    logger_->trace("Assiging Observe CoAP::Option value as 0 for {} message {}",
        toString(type), message_identifier);
    break;
  }
  case LwM2M::MessageType::Cancel_Observation: {
    options += build(OptionNumber::Observe, 1);
    logger_->trace("Assiging Observe CoAP::Option value as 1 for {} message {}",
        toString(type), message_identifier);
    break;
  }
  case LwM2M::MessageType::Cancel_Observation_Composite: {
    options = makeOptions(payload, message_identifier);
    options += build(OptionNumber::Observe, 1);
    logger_->trace("Assiging Observe CoAP::Option value as 1 for {} message {}",
        toString(type), message_identifier);
    // @Attention URI paths for resources to be observed MUST be provided in
    // request payload via CoAP::PayloadPtr
    // CoAP_Encoder::encode(LwM2M::MessageType,LwM2M::PayloadPtr,string) method
    break;
  }
  default: {
    logger_->trace("Could not create any options for {} message {}",
        toString(type), message_identifier);
    break;
  }
  }
  return options;
} // namespace LwM2M

CoAP::Options CoAP_Encoder::makeOptions(
    const LwM2M::PayloadPtr& payload, const string& message_identifier) {
  Options options;

  if (payload) {
    logger_->trace("Begining CoAP::Options pack creation for message {}",
        message_identifier);
    auto data = payload->data_;
    if (holds_alternative<TargetContent>(data)) {
      auto target_content = get<TargetContent>(data);
      logger_->trace("Creating URI Path CoAP::Option for message {} from a "
                     "LwM2M::TargetContent pair",
          message_identifier);
      options = makeURI_PATH(target_content.first);
      logger_->trace(
          "Created URI Path CoAP::Option for message {}", message_identifier);
    } else if (holds_alternative<TargetContentVector>(data)) {
      auto target_contents = get<TargetContentVector>(data);
      logger_->trace("Creating URI Path CoAP::Options for message {} from a "
                     "LwM2M::TargetContentVector",
          message_identifier);
      for (const auto& target_content : target_contents) {
        auto uri_path = makeURI_PATH(target_content.first);
        options.insert(uri_path.begin(), uri_path.end());
      }
      logger_->trace("Created {} URI Path CoAP::Options for message {}",
          options.size(), message_identifier);
    } else if (holds_alternative<ElementID>(data)) {
      auto target = get<ElementID>(data);
      logger_->trace("Creating URI Path CoAP::Option for message {} from a "
                     "LwM2M::ElementID struct",
          message_identifier);
      options = makeURI_PATH(target);
      logger_->trace(
          "Created URI Path CoAP::Option for message {}", message_identifier);
    } else if (holds_alternative<ElementIDs>(data)) {
      auto targets = get<ElementIDs>(data);
      logger_->trace("Creating URI Path CoAP::Options for message {} from a "
                     "LwM2M::ElementIDs vector",
          message_identifier);
      for (auto target : targets) {
        auto uri_path = makeURI_PATH(target);
        options.insert(uri_path.begin(), uri_path.end());
      }
      logger_->trace("Created {} URI Path CoAP::Options for message {}",
          options.size(), message_identifier);
    } else if (holds_alternative<vector<TargetAttribute>>(data)) {
      auto target_attributes = get<vector<TargetAttribute>>(data);
      logger_->trace("Creating URI Path CoAP::Options for message {} from a "
                     "LwM2M::TargetAttributes vector",
          message_identifier);
      for (const auto& target_attribute : target_attributes) {
        auto uri_path = makeURI_PATH(target_attribute.first);
        options.insert(uri_path.begin(), uri_path.end());
      }
      logger_->trace("Created {} URI Path CoAP::Options for message {}",
          options.size(), message_identifier);
    }
  } else {
    logger_->trace("Could not create CoAP::Options Pack. No payload was found "
                   "for message {}",
        message_identifier);
  }

  return options;
}

} // namespace LwM2M
