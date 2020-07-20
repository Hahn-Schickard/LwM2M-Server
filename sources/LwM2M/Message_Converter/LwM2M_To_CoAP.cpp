#include "LwM2M_To_CoAP.hpp"
#include "ContentFormat.hpp"
#include "PlainText.hpp"
#include "RegistrationInterfaceMessages.hpp"
#include "UniquePtrCast.hpp"

using namespace std;
using namespace CoAP;
namespace LwM2M {

unique_ptr<CoAP::Message> createResponse(unique_ptr<Response> input) {
  uint16_t message_id;
  auto payload = make_shared<CoAP::PlainText>(input->payload_);
  vector<std::shared_ptr<CoAP::Option>> options{
      make_shared<CoAP::ContentFormat>(CoAP::ContentFormatType::PLAIN_TEXT)};
  return make_unique<CoAP::Message>(
      input->endpoint_address_, input->endpoint_port_,
      CoAP::Header(CoAP::MessageType::ACKNOWLEDGMENT, payload->size(),
                   static_cast<CoAP::CodeType>(input->response_code_),
                   input->message_id_),
      input->token_, options, move(payload));
}

unique_ptr<CoAP::Message> createRequest(unique_ptr<Message> input) {
  return unique_ptr<CoAP::Message>();
}

LwM2M_To_CoAP::LwM2M_To_CoAP(
    shared_ptr<ThreadsafeQueue<CoAP::Message>> output_queue)
    : output_queue_(output_queue) {}

void LwM2M_To_CoAP::convert(unique_ptr<Message> message) {
  if (message) {
    if (message->response_) {
      auto response =
          createResponse(utility::static_pointer_cast<Response>(move(message)));
      if (response)
        output_queue_->push(move(response));
    }
  } else {
    auto request = createRequest(move(message));
    if (request)
      output_queue_->push(move(request));
  }
}

} // namespace LwM2M