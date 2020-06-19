#include "RegIstrationInterface.hpp"
#include "LoggerRepository.hpp"
#include "XmlParser.hpp"

using namespace std;
using namespace HaSLL;

namespace LwM2M_Model {
RegistrationInterface::RegistrationInterface(const string &configuration_path)
    : logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  try {
    supported_descriptors_ = deserializeModel(configuration_path);
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR,
                 "Received an exception during descriptor deserialization: {}",
                 ex.what());
  }
}

shared_ptr<LwM2M_Message> RegistrationInterface::handleRegisterRequest(
    shared_ptr<Register_Request> request) {
  shared_ptr<LwM2M_Message> result;
  result = make_shared<LwM2M_Message>();
  return result;
}

shared_ptr<LwM2M_Message>
RegistrationInterface::handleUpdateRequest(shared_ptr<Update_Request> request) {
  shared_ptr<LwM2M_Message> result;
  result = make_shared<LwM2M_Message>();
  return result;
}

shared_ptr<LwM2M_Message> RegistrationInterface::handleDeregisterRequest(
    shared_ptr<Deregister_Request> request) {
  shared_ptr<LwM2M_Message> result;
  result = make_shared<LwM2M_Message>();
  return result;
}

shared_ptr<LwM2M_Message> RegistrationInterface::handleRequest(
    shared_ptr<Regirstration_Interface_Message> message) {
  shared_ptr<LwM2M_Message> result;
  switch (message->getMessageType()) {
  case MessageType::REGISTER: {
    result =
        handleRegisterRequest(static_pointer_cast<Register_Request>(message));
    break;
  }
  case MessageType::UPDATE: {
    result = handleUpdateRequest(static_pointer_cast<Update_Request>(message));
    break;
  }
  case MessageType::DEREGISTER: {
    result = handleDeregisterRequest(
        static_pointer_cast<Deregister_Request>(message));
    break;
  }
  default: {
    logger_->log(SeverityLevel::ERROR, "Could not process {} message type",
                 message->getMessageType());
    result = make_shared<LwM2M_Message>();
    break;
  }
  }
  return result;
}
} // namespace LwM2M_Model