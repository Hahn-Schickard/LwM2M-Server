#include "RegIstrationInterface.hpp"
#include "LoggerRepository.hpp"
#include "StringSpliter.hpp"
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

unordered_map<uint32_t, LwM2M_Object>
RegistrationInterface::assignObjectInstances(
    unordered_map<unsigned int, unsigned int> objects) {
  unordered_map<uint32_t, LwM2M_Object> result;
  for (auto &object : objects) {
    uint32_t object_id = object.first;
    uint32_t instance_id = object.second;
    auto it = supported_descriptors_.find(object_id);
    if (it != supported_descriptors_.end()) {
      result.emplace(instance_id, it->second);
    } else {
      logger_->log(SeverityLevel::ERROR,
                   "Object ID {} is not supported by the server.", object_id);
    }
  }
  return result;
}

bool RegistrationInterface::isRegistered(string device_id) {
  return (device_registery_->find(device_id) != device_registery_->end())
             ? true
             : false;
}

shared_ptr<LwM2M_Message> RegistrationInterface::handleRegisterRequest(
    shared_ptr<Register_Request> request) {
  shared_ptr<LwM2M_Message> result;
  if (request) {
    unordered_map<uint32_t, LwM2M_Object> object_instances =
        assignObjectInstances(request->object_instances_map_);
    LwM2M_Device new_device(request->endpoint_name_, request->endpoint_address_,
                            request->endpoint_port_, request->life_time_,
                            request->version_, request->binding_,
                            request->queue_mode_, request->sms_number_,
                            object_instances);
    if (isRegistered(new_device.getDeviceId())) {
      device_registery_->erase(
          device_registery_->find(new_device.getDeviceId()));
      // Notify that device was removed
    }
    device_registery_->emplace(new_device.getDeviceId(), new_device);
    result = make_shared<LwM2M_Response>(
        request->endpoint_address_, request->endpoint_port_, request->token_,
        MessageType::REGISTER, LwM2M_ResponseCode::CREATED,
        utility::convert(new_device.getDeviceId()));
  } else {
    result = make_shared<LwM2M_Response>(
        request->endpoint_address_, request->endpoint_port_, request->token_,
        MessageType::REGISTER, LwM2M_ResponseCode::BAD_REQUEST);
  }
  return result;
}

shared_ptr<LwM2M_Message>
RegistrationInterface::handleUpdateRequest(shared_ptr<Update_Request> request) {
  shared_ptr<LwM2M_Message> result;
  result = make_shared<LwM2M_Response>();
  return result;
}

shared_ptr<LwM2M_Message> RegistrationInterface::handleDeregisterRequest(
    shared_ptr<Deregister_Request> request) {
  shared_ptr<LwM2M_Message> result;
  result = make_shared<LwM2M_Response>();
  return result;
}

shared_ptr<LwM2M_Message> RegistrationInterface::handleRequest(
    shared_ptr<Regirstration_Interface_Message> message) {
  shared_ptr<LwM2M_Message> result;
  switch (message->message_type_) {
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
                 message->message_type_);
    result = make_shared<LwM2M_Message>();
    break;
  }
  }
  return result;
}

shared_ptr<unordered_map<string, LwM2M_Device>>
RegistrationInterface::getDeviceRegistery() {
  return device_registery_;
}
} // namespace LwM2M_Model