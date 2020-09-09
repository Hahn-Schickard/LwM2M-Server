#include "CoAP_Encoder.hpp"
#include "ContentFormat.hpp"
#include "LoggerRepository.hpp"
#include "Option_Builder.hpp"
#include "PlainText.hpp"

using namespace std;
using namespace CoAP;
using namespace HaSLL;

namespace LwM2M {
CoAP_Encoder::CoAP_Encoder(
    shared_ptr<ResponseHandler> response_handler,
    shared_ptr<ThreadsafeUniqueQueue<CoAP::Message>> output_queue)
    : MessageEncoder(response_handler), output_queue_(output_queue),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

ResponseFuture CoAP_Encoder::encode(unique_ptr<ServerRequest_Read> input) {
  ResponseFuture future;
  try {
    vector<shared_ptr<CoAP::Option>> options;
    options.emplace_back(
        build(CoAP::OptionNumber::URI_PATH, to_string(input->object_id_)));
    if (input->object_instance_id_) {
      options.emplace_back(
          build(CoAP::OptionNumber::URI_PATH,
                to_string(input->object_instance_id_.value())));
      if (input->resource_id_) {
        options.emplace_back(build(CoAP::OptionNumber::URI_PATH,
                                   to_string(input->resource_id_.value())));
        if (input->resoruce_instance_id_) {
          options.emplace_back(
              build(CoAP::OptionNumber::URI_PATH,
                    to_string(input->resoruce_instance_id_.value())));
        }
      }
      options.emplace_back(
          build(CoAP::OptionNumber::ACCEPT,
                to_string(static_cast<uint16_t>(ContentFormatType::TLV))));
    }
    auto header =
        CoAP::Header(CoAP::MessageType::CONFIRMABLE, 8, CodeType::GET);
    auto msg = make_unique<CoAP::Message>(input->endpoint_address_,
                                          input->endpoint_port_, header,
                                          options, shared_ptr<PayloadFormat>());
    future = getResponseHandler()->getFuture(msg->getToken());
    output_queue_->push(move(msg));
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR, ex.what());
  }
  return future;
}

void CoAP_Encoder::encode(unique_ptr<ServerResponse_Register> input) {
  try {
    vector<shared_ptr<CoAP::Option>> options;
    for (auto location_part : input->location_)
      options.emplace_back(
          build(CoAP::OptionNumber::LOCATION_PATH, location_part));
    auto header =
        CoAP::Header(CoAP::MessageType::ACKNOWLEDGMENT, input->token_.size(),
                     static_cast<CoAP::CodeType>(input->response_code_),
                     input->message_id_.value());
    auto msg = make_unique<CoAP::Message>(
        input->endpoint_address_, input->endpoint_port_, header, input->token_,
        options, shared_ptr<PayloadFormat>());
    output_queue_->push(move(msg));
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR, ex.what());
  }
}

void CoAP_Encoder::encode(std::unique_ptr<Response> input) {
  try {
    auto header =
        CoAP::Header(CoAP::MessageType::ACKNOWLEDGMENT, input->token_.size(),
                     static_cast<CoAP::CodeType>(input->response_code_),
                     input->message_id_.value());
    auto msg = make_unique<CoAP::Message>(
        input->endpoint_address_, input->endpoint_port_, header, input->token_,
        vector<shared_ptr<CoAP::Option>>(), shared_ptr<PayloadFormat>());
    output_queue_->push(move(msg));
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR, ex.what());
  }
}

} // namespace LwM2M