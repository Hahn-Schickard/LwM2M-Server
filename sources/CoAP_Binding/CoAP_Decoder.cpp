#include "CoAP_Decoder.hpp"
#include "CoAP_ContentTypes.hpp"
#include "LoggerRepository.hpp"
#include "RegistrationInterfaceRequestsBuilder.hpp"
#include "Utility.hpp"

#include "CoAP/CoRE_Link.hpp"
#include "CoAP/OctetStream.hpp"
#include "CoAP/OptionBuilder.hpp"
#include "CoAP/PlainText.hpp"
#include "TLV.hpp"

using namespace std;
using namespace CoAP;
using namespace HaSLL;

namespace LwM2M {
ResponseCode toResponseCode(CoAP::CodeType code) {
  // I will suffer, only if I f*ed up the CoAP::Socket implementation
  return static_cast<ResponseCode>(code);
}
LwM2M::PayloadPtr toPayload(PlainText content) {
  return make_shared<LwM2M::Payload>(
      make_shared<DataFormat>(content.toString()));
}

ElementID toElementID(vector<string> targets) {
  switch (targets.size()) {
  case 1: {
    auto object_id = toUnsignedInteger(targets[0], IntegerBase::BASE_10);
    return ElementID(object_id);
  }
  case 2: {
    auto object_id = toUnsignedInteger(targets[0], IntegerBase::BASE_10);
    auto instance_id = toUnsignedInteger(targets[1], IntegerBase::BASE_10);
    return ElementID(object_id, instance_id);
  }
  case 3: {
    auto object_id = toUnsignedInteger(targets[0], IntegerBase::BASE_10);
    auto instance_id = toUnsignedInteger(targets[1], IntegerBase::BASE_10);
    auto resource_id = toUnsignedInteger(targets[2], IntegerBase::BASE_10);
    return ElementID(object_id, instance_id, resource_id);
  }
  case 4: {
    auto object_id = toUnsignedInteger(targets[0], IntegerBase::BASE_10);
    auto instance_id = toUnsignedInteger(targets[1], IntegerBase::BASE_10);
    auto resource_id = toUnsignedInteger(targets[2], IntegerBase::BASE_10);
    auto resource_instance_id =
        toUnsignedInteger(targets[3], IntegerBase::BASE_10);
    return ElementID(object_id, instance_id, resource_id, resource_instance_id);
  }
  default: {
    string error_msg =
        "Unsupported CoRE Link Target format. Target is split into " +
        to_string(targets.size()) + " parts, expected 1,2,3 or 4 parts.";
    throw runtime_error(error_msg);
  }
  }
}

LwM2M::PayloadPtr toPayload(CoRE_Links content) {
  ElementIDs result;
  for (auto link : content.getLinks()) {
    auto target = link.splitTarget('/');
    // ignore empty root element
    if (target.begin()->empty()) {
      target.erase(target.begin());
    }
    // @TODO: possibly silently ignore targets that throw runtime_error with a
    // try-catch block
    result.emplace_back(toElementID(target));
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
      }
      return make_shared<LwM2M::Payload>(targets_and_values);
    }
  }
  return LwM2M::PayloadPtr();
}

LwM2M::PayloadPtr toPayload(OctetStream content) {
  return make_shared<LwM2M::Payload>(
      make_shared<DataFormat>(content.getValue()));
}

CoAP_Decoder::CoAP_Decoder()
    : logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

CoAP_Decoder::~CoAP_Decoder() {
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

LwM2M::PayloadPtr CoAP_Decoder::decode(CoAP::ContentFormatPtr content_format,
                                       CoAP::PayloadPtr payload) {
  if (content_format) {
    switch (content_format->getIndex()) {
    case ContentFormatEncodings::PlainText::index: {
      auto text = CoAP::decode<PlainText>(payload);
      return toPayload(text);
    }
    case ContentFormatEncodings::CoRE_Link::index: {
      auto core_links = CoAP::decode<CoRE_Links>(payload);
      return toPayload(core_links);
    }
    case ContentFormatEncodings::LwM2M_TLV::index: {
      auto tlv = CoAP::decode<TLV_Pack>(payload);
      return toPayload(tlv);
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
      auto octets = CoAP::decode<OctetStream>(payload);
      return toPayload(octets);
    }
    default: {
      throw runtime_error("Unhandled Content Format with index :" +
                          content_format->getIndex());
    }
    }
  } else {
    throw runtime_error("Failed to decode CoAP Payload type. Content Format "
                        "option can not be empty.");
  }
}

template <>
ClientResponsePtr
CoAP_Decoder::decode<ClientResponse>(CoAP::MessagePtr message) {
  auto endpoint =
      make_shared<Endpoint>(message->getAddressIP(), message->getAddressPort());
  logger_->log(
      SeverityLevel::TRACE,
      "Creating a Client Response from {}:{} CoAP response with Token {}",
      message->getAddressIP(), message->getAddressPort(),
      message->getTokenAsHexString());
  try {
    auto code = toResponseCode(message->getHeader()->getCodeType());
    logger_->log(
        SeverityLevel::TRACE,
        "Assigning {} Response Code to Client Response from {}:{} CoAP "
        "response with Token {}",
        toString(code), message->getAddressIP(), message->getAddressPort(),
        message->getTokenAsHexString());
    if (auto payload = message->getPayload()) {
      logger_->log(
          SeverityLevel::TRACE,
          "Assigning payload of {} bytes to Client Response from {}:{} CoAP "
          "response with Token {}",
          payload->getBytes().size(), message->getAddressIP(),
          message->getAddressPort(), message->getTokenAsHexString());
      LwM2M::PayloadPtr content;
      auto options = message->getOptions();
      logger_->log(SeverityLevel::TRACE,
                   "Looking for CONTENT_FORMAT Option in {}:{} CoAP "
                   "response with Token {} Payload.",
                   message->getAddressIP(), message->getAddressPort(),
                   message->getTokenAsHexString());
      auto it = options.find(OptionNumber::CONTENT_FORMAT);
      if (it != options.end()) {
        logger_->log(SeverityLevel::TRACE,
                     "CONTENT_FORMAT Option found in {}:{} CoAP "
                     "response with Token {} Payload. Casting it as "
                     "CoAP::ContentFormat Class",
                     message->getAddressIP(), message->getAddressPort(),
                     message->getTokenAsHexString());
        if (auto content_format_option = it->second) {
          auto content_format =
              dynamic_pointer_cast<CoAP::ContentFormat>(content_format_option);
          content = decode(content_format, payload);
        } else {
          throw runtime_error(
              "Content Format option value can not be a nullptr.");
        }
        if (content) {
          logger_->log(
              SeverityLevel::TRACE,
              "Payload {} of {} bytes assigned as content for Client Response "
              "from {}:{} CoAP "
              "response with Token {}",
              toString(content->media_type_), content->size(),
              message->getAddressIP(), message->getAddressPort(),
              message->getTokenAsHexString());
        } else {
          logger_->log(SeverityLevel::WARNNING,
                       "Payload of {} bytes could not be converted into a "
                       "Content Type for Client Response "
                       "from {}:{} CoAP "
                       "response with Token {}",
                       payload->getBytes().size(), message->getAddressIP(),
                       message->getAddressPort(),
                       message->getTokenAsHexString());
        }
        return make_shared<ClientResponse>(endpoint, code, content);
      }
    }
    return make_shared<ClientResponse>(endpoint, code);
  } catch (exception &ex) {
    logger_->log(SeverityLevel::CRITICAL,
                 "Caught an unhandled exception, while building a "
                 "ClientResponse from message {} from {}:{}. Exception: {}",
                 message->getTokenAsHexString(), message->getAddressIP(),
                 message->getAddressPort(), ex.what());
    return make_shared<ClientResponse>(endpoint, ResponseCode::BAD_REQUEST);
  }
}

template <>
RegisterRequestPtr
CoAP_Decoder::decode<RegisterRequest>(CoAP::MessagePtr message) {
  return buildRegisterRequest(message);
}

template <>
UpdateRequestPtr CoAP_Decoder::decode<UpdateRequest>(CoAP::MessagePtr message) {
  return buildUpdateRequest(message);
}

template <>
DeregisterRequestPtr
CoAP_Decoder::decode<DeregisterRequest>(CoAP::MessagePtr message) {
  return buildDeregisterRequest(message);
}
} // namespace LwM2M