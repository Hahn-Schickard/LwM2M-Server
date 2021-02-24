#include "CoAP_RequestsManager.hpp"
#include "CoAP/Message.hpp"
#include "CoAP/OptionBuilder.hpp"
#include "CoAP/PlainText.hpp"
#include "CoAP_ContentTypes.hpp"
#include "TLV.hpp"
#include "Utility.hpp"

using namespace std;
using namespace CoAP;

#define TOKEN_SIZE 8

namespace LwM2M {

CodeType toCodeType(LwM2M::MessageType type) {
  switch (type) {
  case MessageType::OBSERVE:
  case MessageType::CANCEL_OBSERVATION:
  case MessageType::DISCOVER:
  case MessageType::READ: {
    return CodeType::GET;
  }
  case MessageType::OBSERVE_COMPOSITE:
  case MessageType::CANCEL_OBSERVATION_COMPOSITE:
  case MessageType::READ_COMPOSITE: {
    return CodeType::FETCH;
  }
  case MessageType::WRITE:
  case MessageType::WRITE_ATTRIBUTES: {
    return CodeType::PUT;
  }
  case MessageType::WRITE_COMPOSITE: {
    return CodeType::iPATCH;
  }
  case MessageType::CREATE:
  case MessageType::EXECUTE: {
    return CodeType::POST;
  }
  case MessageType::DELETE: {
    return CodeType::DELETE;
  }
  default: { throw logic_error("Message is not a valid ServerRequest."); }
  }
}

HeaderPtr makeHeader(ServerRequestPtr request) {
  uint8_t request_size = TOKEN_SIZE + request->payload_->size();
  return make_shared<Header>(CoAP::MessageType::CONFIRMABLE, request_size,
                             toCodeType(request->message_type_));
}

CoAP::Options makeURI_PATH(ElmentIdVariant target) {
  Options options;
  for (auto uri : toStrings(target)) {
    options.emplace(OptionNumber::URI_PATH, build(OptionNumber::URI_PATH, uri));
  }
  return options;
}

CoAP::Options toOptions(PayloadPtr payload) {
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
    } else if (holds_alternative<ElmentIdVariant>(data)) {
      auto target = get<ElmentIdVariant>(data);
      options = makeURI_PATH(target);
    } else if (holds_alternative<vector<ElmentIdVariant>>(data)) {
      auto targets = get<vector<ElmentIdVariant>>(data);
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

  if (request->interface_ == InterfaceType::DEVICE_MANAGMENT) {
    switch (request->message_type_) {
    case MessageType::READ: {
      options.emplace(
          OptionNumber::ACCEPT,
          build(OptionNumber::ACCEPT,
                to_string(ContentFormatEncodings::LwM2M_TLV::index)));
      break;
    }
    case MessageType::READ_COMPOSITE: {
      options.emplace(
          OptionNumber::CONTENT_FORMAT,
          build(OptionNumber::CONTENT_FORMAT,
                to_string(ContentFormatEncodings::LwM2M_CBOR::index)));
      break;
    }
    case MessageType::WRITE: {
      options.emplace(
          OptionNumber::CONTENT_FORMAT,
          build(OptionNumber::CONTENT_FORMAT,
                to_string(ContentFormatEncodings::LwM2M_TLV::index)));
      break;
    }
    case MessageType::WRITE_COMPOSITE: {
      options.emplace(
          OptionNumber::CONTENT_FORMAT,
          build(OptionNumber::CONTENT_FORMAT,
                to_string(ContentFormatEncodings::LwM2M_CBOR::index)));
      break;
    }
    case MessageType::EXECUTE: {
      // check if there is some arguments first!
      options.emplace(
          OptionNumber::CONTENT_FORMAT,
          build(OptionNumber::CONTENT_FORMAT,
                to_string(ContentFormatEncodings::PlainText::index)));
      break;
    }
    case MessageType::CREATE: {
      options.emplace(
          OptionNumber::CONTENT_FORMAT,
          build(OptionNumber::CONTENT_FORMAT,
                to_string(ContentFormatEncodings::LwM2M_CBOR::index)));
      break;
    }
    default: { break; }
    }
  } else if (request->interface_ == InterfaceType::INFORMATION_REPORTING) {
    switch (request->message_type_) {
    case MessageType::OBSERVE_COMPOSITE: {
      options.emplace(
          OptionNumber::CONTENT_FORMAT,
          build(OptionNumber::CONTENT_FORMAT,
                to_string(ContentFormatEncodings::LwM2M_JSON::index)));
      options.emplace(
          OptionNumber::ACCEPT,
          build(OptionNumber::ACCEPT,
                to_string(ContentFormatEncodings::LwM2M_CBOR::index)));
      [[fallthrough]];
    }
    case MessageType::OBSERVE: {
      options.emplace(OptionNumber::OBSERVE,
                      build(OptionNumber::OBSERVE, to_string(true)));
      break;
    }
    case MessageType::CANCEL_OBSERVATION:
    case MessageType::CANCEL_OBSERVATION_COMPOSITE: {
      options.emplace(OptionNumber::OBSERVE,
                      build(OptionNumber::OBSERVE, to_string(false)));
      break;
    }
    default: { break; }
    }
  }

  return options;
}

CoAP::PayloadPtr makeTLV(PayloadPtr payload) {
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

CoAP::PayloadPtr makeLwM2M_CBOR(PayloadPtr payload) {
  if (payload) {
    auto data = payload->data_;
    if (holds_alternative<vector<TargetContent>>(data)) {
      // encode in LwM2M_CBOR format here
    }
  }
  return CoAP::PayloadPtr();
}

CoAP::PayloadPtr makePlainText(PayloadPtr payload) {
  if (payload) {
    auto data = payload->data_;
    if (holds_alternative<DataFormatPtr>(data)) {
      auto arguments = get<DataFormatPtr>(data)->get<string>();
      return encode<PlainText>(arguments);
    }
  }
  return CoAP::PayloadPtr();
}

CoAP::PayloadPtr makePayload(ServerRequestPtr request) {
  switch (request->message_type_) {
  case MessageType::WRITE: {
    return makeTLV(request->payload_);
  }
  case MessageType::WRITE_COMPOSITE: {
    return makeLwM2M_CBOR(request->payload_);
  }
  case MessageType::EXECUTE: {
    return makePlainText(request->payload_);
  }
  default: { return CoAP::PayloadPtr(); }
  }
}

CoAP_RequestsManager::CoAP_RequestsManager(ResponseHandlerPtr response_handler,
                                           CoAP::SocketPtr socket)
    : RequestsManagerInterface(response_handler), socket_(socket) {}

uint64_t CoAP_RequestsManager::dispatch(ServerRequestPtr request) {
  auto header = makeHeader(request);
  auto message = make_shared<CoAP::Message>(
      request->endpoint_->endpoint_address_, request->endpoint_->endpoint_port_,
      move(header));
  message->generateToken();
  auto options = makeOptions(request);
  if (!options.empty()) {
    *message += options;
  }
  auto payload = makePayload(request);
  if (payload) {
    *message += payload;
  }

  auto sent = socket_->send(message);
  sent.get();

  return generateHash(message);
}
} // namespace LwM2M
