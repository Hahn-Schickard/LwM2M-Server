#include "CoAP_Encoder.hpp"
#include "ContentFormat.hpp"
#include "LoggerRepository.hpp"
#include "Option_Builder.hpp"
#include "PlainText.hpp"
#include "RegistrationInterfaceMessages.hpp"

using namespace std;
using namespace CoAP;
using namespace HaSLL;

namespace LwM2M {
CoAP_Encoder::CoAP_Encoder(
    shared_ptr<ThreadsafeUniqueQueue<CoAP::Message>> output_queue)
    : MessageEncoder(), output_queue_(output_queue),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {}

void CoAP_Encoder::encode(std::unique_ptr<Read_Request> input) {
  try {
    vector<shared_ptr<CoAP::Option>> options;
    options.emplace_back(build(CoAP::OptionNumber::ACCEPT, "CORE_LINK"));
    options.emplace_back(
        build(CoAP::OptionNumber::LOCATION_PATH, to_string(input->object_id_)));
    if (input->object_instance_id_) {
      options.emplace_back(
          build(CoAP::OptionNumber::LOCATION_PATH,
                to_string(input->object_instance_id_.value())));
      if (input->resource_id_) {
        options.emplace_back(build(CoAP::OptionNumber::LOCATION_PATH,
                                   to_string(input->resource_id_.value())));
        if (input->resoruce_instance_id_) {
          options.emplace_back(
              build(CoAP::OptionNumber::LOCATION_PATH,
                    to_string(input->resoruce_instance_id_.value())));
        }
      }
    }
    output_queue_->push(make_unique<CoAP::Message>(
        input->endpoint_address_, input->endpoint_port_,
        CoAP::Header(CoAP::MessageType::ACKNOWLEDGMENT, 8, CodeType::GET),
        options, shared_ptr<PayloadFormat>()));
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR, ex.what());
  }
}

void CoAP_Encoder::encode(unique_ptr<Register_Response> input) {
  try {
    vector<shared_ptr<CoAP::Option>> options;
    for (auto location_part : input->location_)
      options.emplace_back(
          build(CoAP::OptionNumber::LOCATION_PATH, location_part));
    output_queue_->push(make_unique<CoAP::Message>(
        input->endpoint_address_, input->endpoint_port_,
        CoAP::Header(CoAP::MessageType::ACKNOWLEDGMENT, input->token_.size(),
                     static_cast<CoAP::CodeType>(input->response_code_),
                     input->message_id_.value()),
        options, shared_ptr<PayloadFormat>()));
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR, ex.what());
  }
}

void CoAP_Encoder::encode(std::unique_ptr<Response> input) {
  try {
    output_queue_->push(make_unique<CoAP::Message>(
        input->endpoint_address_, input->endpoint_port_,
        CoAP::Header(CoAP::MessageType::ACKNOWLEDGMENT, input->token_.size(),
                     static_cast<CoAP::CodeType>(input->response_code_),
                     input->message_id_.value()),
        input->token_, vector<shared_ptr<CoAP::Option>>(),
        shared_ptr<PayloadFormat>()));
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR, ex.what());
  }
}

} // namespace LwM2M