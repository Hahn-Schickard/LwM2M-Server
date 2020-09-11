#include "CoAP_DeviceManagmentMessageDecoder.hpp"
#include "CoAP_Option.hpp"
#include "CoRE_Link.hpp"
#include "LoggerRepository.hpp"
#include "LwM2M_Opaque.hpp"
#include "LwM2M_PlainText.hpp"
#include "LwM2M_TLV.hpp"
#include "PlainText.hpp"

#include <stdexcept>

using namespace std;
using namespace CoAP;
using namespace HaSLL;

namespace LwM2M {

ResponseCode convert(CoAP::CodeType code_type) {
  switch (code_type) {
  case CoAP::CodeType::OK: {
    return ResponseCode::OK;
  }
  case CoAP::CodeType::CREATED: {
    return ResponseCode::CREATED;
  }
  case CoAP::CodeType::DELETED: {
    return ResponseCode::DELETED;
  }
  case CoAP::CodeType::CHANGED: {
    return ResponseCode::CHANGED;
  }
  case CoAP::CodeType::CONTENT: {
    return ResponseCode::CONTENT;
  }
  case CoAP::CodeType::CONTINUE: {
    return ResponseCode::CONTINUE;
  }
  case CoAP::CodeType::BAD_REQUEST: {
    return ResponseCode::BAD_REQUEST;
  }
  case CoAP::CodeType::UNAUTHORIZED: {
    return ResponseCode::UNAUTHORIZED;
  }
  case CoAP::CodeType::FORBIDDEN: {
    return ResponseCode::FORBIDDEN;
  }
  case CoAP::CodeType::NOT_FOUND: {
    return ResponseCode::NOT_FOUND;
  }
  case CoAP::CodeType::METHOD_NOT_ALLOWED: {
    return ResponseCode::METHOD_NOT_ALLOWED;
  }
  case CoAP::CodeType::NOT_ACCEPTABLE: {
    return ResponseCode::NOT_ACCEPTABLE;
  }
  case CoAP::CodeType::REQUEST_ENTITY_INCOMPLETE: {
    return ResponseCode::REQUEST_ENTITY_TOO_LARGE;
  }
  case CoAP::CodeType::PRECOGNITION_FAILED: {
    return ResponseCode::PRECOGNITION_FAILED;
  }
  case CoAP::CodeType::REQUEST_ENTITY_TOO_LARGE: {
    return ResponseCode::REQUEST_ENTITY_TOO_LARGE;
  }
  case CoAP::CodeType::UNSUPPORTED_CONTENT_FORMAT: {
    return ResponseCode::UNSUPPORTED_CONTENT_FORMAT;
  }
  default: { return ResponseCode::UNHANDLED; }
  }
}

shared_ptr<DataFormat>
decodeResponsePayload(shared_ptr<PayloadFormat> payload) {
  switch (payload->getContentFormatType().getAsShort()) {
  case static_cast<uint16_t>(ContentFormatType::PLAIN_TEXT): {
    return make_shared<PlainText>(payload->getBytes());
  }
  case static_cast<uint16_t>(ContentFormatType::OPAQUE): {
    return make_shared<Opaque>(payload->getBytes());
  }
  case static_cast<uint16_t>(ContentFormatType::TLV): {
    return make_shared<TLV_Pack>(payload->getBytes());
  }
  case static_cast<uint16_t>(ContentFormatType::JSON):
  case static_cast<uint16_t>(ContentFormatType::CBOR):
  case static_cast<uint16_t>(ContentFormatType::UNRECOGNIZED):
  default: { return shared_ptr<DataFormat>(); }
  }
}

CoAP_DeviceManagmentMessageDecoder::CoAP_DeviceManagmentMessageDecoder(
    std::shared_ptr<ResponseHandler> response_handler,
    std::shared_ptr<ThreadsafeHashSet<CoAP::Message>> message_buffer)
    : response_handler_(response_handler), message_buffer_(message_buffer),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

unique_ptr<Response> CoAP_DeviceManagmentMessageDecoder::makeResponse(
    shared_ptr<CoAP::Message> message) {
  unique_ptr<Response> response;
  if (message->getHeader().getCodeType() == CoAP::CodeType::CONTENT) {
    auto payload = decodeResponsePayload(message->getBody());
    if (payload) {
      response = make_unique<Response>(
          message->getReceiverIP(), message->getReceiverPort(),
          message->getHeader().getMessageID(), message->getToken(),
          MessageType::NOT_RECOGNIZED, ResponseCode::CONTENT, payload);
    } else {
      string error_msg =
          "Received a Content message with unrecognized payload: " +
          to_string(message->getBody()->getContentFormatType().getAsShort()) +
          " from " + message->getReceiverIP() + ":" +
          to_string(message->getReceiverPort());
      throw runtime_error(move(error_msg));
    }
  } else if (message->getHeader().getCodeType() == CoAP::CodeType::CONTINUE) {
    // @TODO: handle segmented packets
  } else if (message->getHeader().getCodeType() != CoAP::CodeType::UNHANDLED) {
    response = make_unique<Response>(
        message->getReceiverIP(), message->getReceiverPort(),
        message->getHeader().getMessageID(), message->getToken(),
        MessageType::NOT_RECOGNIZED,
        convert(message->getHeader().getCodeType()));
  }
  return response;
}

bool CoAP_DeviceManagmentMessageDecoder::tryDecode(
    shared_ptr<CoAP::Message> message) {
  if (message) {
    if (message->getHeader().getMesageType() ==
        CoAP::MessageType::ACKNOWLEDGMENT) {
      auto response = makeResponse(message);
      if (response) {
        response_handler_->setFuture(move(response));
        return true;
      }
    }
  }
  return false;
}

void CoAP_DeviceManagmentMessageDecoder::run() {
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