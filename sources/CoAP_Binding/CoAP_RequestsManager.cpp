#include "CoAP_RequestsManager.hpp"
#include "CoAP/Message.hpp"

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

CoAP::Options makeOptions(ServerRequestPtr request) {
  // based on ServerRequest populate Option vector
  return CoAP::Options();
}

CoAP::PayloadPtr makePayload(ServerRequestPtr request) {
  if (request->payload_) {
    // encode to coap payload here
    return CoAP::PayloadPtr();
  } else {
    return CoAP::PayloadPtr();
  }
}

CoAP_RequestsManager::CoAP_RequestsManager(ResponseHandlerPtr response_handler,
                                           shared_ptr<CoAP::Server> server)
    : RequestsManagerInterface(response_handler), server_(server) {}

uint64_t CoAP_RequestsManager::dispatch(ServerRequestPtr request) {
  auto header = makeHeader(request);
  auto message = make_shared<CoAP::Message>(
      request->endpoint_->endpoint_address_, request->endpoint_->endpoint_port_,
      move(header));
  // @TODO: expose generateToken method in CoAPS4Cpp
  // *message += message->generateToken();
  // uint64_t message_identifier = hash<vector<unit8_t>{}(message->getToken());
  auto options = makeOptions(request);
  if (!options.empty()) {
    *message += options;
  }
  auto payload = makePayload(request);
  if (payload) {
    *message += payload;
  }
  auto sent = server_->send(message);
  sent.get();
  return 0;
}
} // namespace LwM2M
