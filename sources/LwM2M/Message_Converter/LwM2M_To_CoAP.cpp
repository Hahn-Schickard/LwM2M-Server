#include "LwM2M_To_CoAP.hpp"
#include "ContentFormat.hpp"
#include "Option_Builder.hpp"
#include "PlainText.hpp"
#include "RegistrationInterfaceMessages.hpp"
#include "UniquePtrCast.hpp"

using namespace std;
using namespace CoAP;
namespace LwM2M {

unique_ptr<CoAP::Message> createResponse(unique_ptr<Register_Response> input) {
  vector<shared_ptr<CoAP::Option>> options;
  for (auto location_part : input->location_)
    options.emplace_back(
        build(CoAP::OptionNumber::LOCATION_PATH, location_part));
  return make_unique<CoAP::Message>(
      input->endpoint_address_, input->endpoint_port_,
      CoAP::Header(CoAP::MessageType::ACKNOWLEDGMENT, input->token_.size(),
                   static_cast<CoAP::CodeType>(input->response_code_),
                   input->message_id_),
      input->token_, options, shared_ptr<PayloadFormat>());
}

unique_ptr<CoAP::Message> createResponse(unique_ptr<Response> input) {
  auto payload = make_shared<CoAP::PlainText>(input->payload_);
  return make_unique<CoAP::Message>(
      input->endpoint_address_, input->endpoint_port_,
      CoAP::Header(CoAP::MessageType::ACKNOWLEDGMENT,
                   input->token_.size() + payload->size(),
                   static_cast<CoAP::CodeType>(input->response_code_),
                   input->message_id_),
      input->token_, vector<shared_ptr<CoAP::Option>>(), move(payload));
}

unique_ptr<CoAP::Message> createRequest(unique_ptr<Message> input) {
  return unique_ptr<CoAP::Message>();
}

LwM2M_To_CoAP::LwM2M_To_CoAP(
    shared_ptr<QueueInterface<CoAP::Message>> output_queue)
    : output_queue_(output_queue) {}

void LwM2M_To_CoAP::convert(unique_ptr<Message> message) {
  if (message) {
    if (message->response_) {
      // @TODO make thsi generic
      auto response = createResponse(
          utility::static_pointer_cast<Register_Response>(move(message)));
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