#include "RegistrationInterface.hpp"
#include "LoggerRepository.hpp"
#include "StringSpliter.hpp"
#include "UniquePtrCast.hpp"
#include "XmlParser.hpp"

using namespace std;
using namespace HaSLL;

namespace LwM2M {

RegistrationInterface::RegistrationInterface(
    shared_ptr<ThreadsafeQueue<Message>> outgoing_message_queue,
    shared_ptr<ThreadsafeQueue<Regirstration_Interface_Message>>
        incoming_message_queue,
    shared_ptr<unordered_map<string, shared_ptr<Device>>> device_registery,
    const string &configuration_path)
    : InterfaceRunner(outgoing_message_queue),
      incoming_message_queue_(incoming_message_queue),
      device_registery_(device_registery),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  try {
    supported_descriptors_ = deserializeModel(configuration_path);
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR,
                 "Received an exception during descriptor deserialization: {}",
                 ex.what());
  }
}

void RegistrationInterface::run() {
  while (!stopRequested()) {
    try {
      auto message = incoming_message_queue_->try_pop();
      if (message) {
        auto response = handleRequest(move(message));
        if (response) {
          outgoing_message_queue_->push(move(response));
        }
      }
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR,
                   "Received an exception while processing messages: {}",
                   ex.what());
    }
  }
}

unordered_map<uint32_t, ObjectDescriptor>
RegistrationInterface::assignObjectInstances(
    unordered_map<unsigned int, unsigned int> objects) {
  unordered_map<uint32_t, ObjectDescriptor> result;
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

unique_ptr<Message> RegistrationInterface::handleRegisterRequest(
    unique_ptr<Register_Request> request) {
  unique_ptr<Message> result;
  if (request) {
    unordered_map<uint32_t, ObjectDescriptor> object_instances =
        assignObjectInstances(request->object_instances_map_);
    auto new_device = make_shared<Device>(
        request->endpoint_name_, request->endpoint_address_,
        request->endpoint_port_, request->life_time_, request->version_,
        request->binding_, request->queue_mode_, request->sms_number_,
        object_instances);
    if (isRegistered(new_device->getDeviceId())) {
      device_registery_->erase(
          device_registery_->find(new_device->getDeviceId()));
      // Notify that device was removed
    }
    device_registery_->emplace(new_device->getDeviceId(), new_device);
    result = make_unique<Response>(
        request->endpoint_address_, request->endpoint_port_,
        request->message_id_, request->token_, MessageType::REGISTER,
        ResponseCode::CREATED, utility::convert(new_device->getDeviceId()));
  } else {
    result = make_unique<Response>(
        request->endpoint_address_, request->endpoint_port_,
        request->message_id_, request->token_, MessageType::REGISTER,
        ResponseCode::BAD_REQUEST);
  }
  return result;
}

unique_ptr<Message>
RegistrationInterface::handleUpdateRequest(unique_ptr<Update_Request> request) {
  unique_ptr<Message> result;
  return result;
}

unique_ptr<Message> RegistrationInterface::handleDeregisterRequest(
    unique_ptr<Deregister_Request> request) {
  unique_ptr<Message> result;
  return result;
}

unique_ptr<Message> RegistrationInterface::handleRequest(
    unique_ptr<Regirstration_Interface_Message> message) {
  unique_ptr<Message> result;
  switch (message->message_type_) {
  case MessageType::REGISTER: {
    result = handleRegisterRequest(
        utility::static_pointer_cast<Register_Request>(move(message)));
    break;
  }
  case MessageType::UPDATE: {
    result = handleUpdateRequest(
        utility::static_pointer_cast<Update_Request>(move(message)));
    break;
  }
  case MessageType::DEREGISTER: {
    result = handleDeregisterRequest(
        utility::static_pointer_cast<Deregister_Request>(move(message)));
    break;
  }
  default: {
    logger_->log(SeverityLevel::ERROR, "Could not process {} message type",
                 message->message_type_);
    result = unique_ptr<Message>();
    break;
  }
  }
  return result;
}
} // namespace LwM2M