#include "RegistryHandler.hpp"
#include "LoggerRepository.hpp"
#include "StringSpliter.hpp"
#include "XmlParser.hpp"

#include <stdexcept>

using namespace std;
using namespace HaSLL;

namespace LwM2M {

struct ObjectDescriptorNotSupported : runtime_error {
  ObjectDescriptorNotSupported(string const &message)
      : runtime_error(message) {}
};

RegistryHandler::RegistryHandler(
    shared_ptr<unordered_map<string, shared_ptr<Device>>> device_registery,
    shared_ptr<MessageEncoder> encoder, const string &configuration_path)
    : ObserverPattern::EventSource<RegistryEvent>(),
      device_registery_(device_registery), encoder_(encoder),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  try {
    supported_descriptors_ = deserializeModel(configuration_path);
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR,
                 "Received an exception during descriptor deserialization: {}",
                 ex.what());
  }
}

unordered_map<uint32_t, ObjectDescriptorPair>
RegistryHandler::assignObjectInstances(
    unordered_map<unsigned int, vector<unsigned int>> objects) {
  unordered_map<uint32_t, ObjectDescriptorPair> result;
  for (auto &object : objects) {
    auto object_id = object.first;
    auto instance_ids = object.second;
    auto it = supported_descriptors_.find(object_id);
    if (it != supported_descriptors_.end()) {
      result.emplace(object_id, ObjectDescriptorPair(it->second, instance_ids));
    } else {
      string error_msg = "Object ID " + to_string(object_id) +
                         " is not supported by the server.";
      throw ObjectDescriptorNotSupported(move(error_msg));
    }
  }
  return result;
}

bool RegistryHandler::isRegistered(string device_id) {
  return (device_registery_->find(device_id) != device_registery_->end())
             ? true
             : false;
}

bool RegistryHandler::handleRequest(
    unique_ptr<ClientRequest_Register> request) {
  unique_ptr<ServerResponse_Register> result;
  shared_ptr<RegistryEvent> event;
  try {
    auto object_instances =
        assignObjectInstances(request->object_instances_map_);
    auto new_device = make_shared<Device>(
        encoder_, request->endpoint_name_, request->endpoint_address_,
        request->endpoint_port_, request->life_time_, request->version_,
        request->binding_, request->queue_mode_, request->sms_number_,
        object_instances);
    if (isRegistered(new_device->getDeviceId()))
      device_registery_->erase(
          device_registery_->find(new_device->getDeviceId()));
    device_registery_->emplace(new_device->getDeviceId(), new_device);
    event = make_shared<RegistryEvent>(RegistryEvent{
        RegistryEventType::REGISTERED, new_device->getDeviceId(), new_device});
    result = make_unique<ServerResponse_Register>(
        request->endpoint_address_, request->endpoint_port_,
        request->message_id_.value(), request->token_, MessageType::REGISTER,
        ResponseCode::CREATED, vector<string>{"rd", new_device->getDeviceId()});
  } catch (ObjectDescriptorNotSupported &ex) {
    logger_->log(SeverityLevel::ERROR, ex.what());
    result = make_unique<ServerResponse_Register>(
        request->endpoint_address_, request->endpoint_port_,
        request->message_id_.value(), request->token_, MessageType::REGISTER,
        ResponseCode::BAD_REQUEST);
  }
  encoder_->encode(move(result));
  if (event)
    notify(move(event));
  return true;
}

bool RegistryHandler::handleRequest(unique_ptr<ClientRequest_Update> request) {
  try {
    auto device = device_registery_->at(request->location_);
    bool updated = false;
    if (device) {
      if (request->binding_) {
        device->updateBinding(request->binding_.value());
        updated = true;
      }
      if (request->lifetime_) {
        device->updateLifetime(request->lifetime_.value());
        updated = true;
      }
      if (request->sms_number_) {
        device->updateSMS_Number(request->sms_number_.value());
        updated = true;
      }
      if (!request->object_instances_map_.empty()) {
        auto object_instances =
            assignObjectInstances(request->object_instances_map_);
        device->updateObjectsMap(object_instances);
        updated = true;
      }
      if (updated)
        notify(make_shared<RegistryEvent>(RegistryEvent{
            RegistryEventType::UPDATED, request->location_, device}));
      encoder_->encode(make_unique<Response>(
          request->endpoint_address_, request->endpoint_port_,
          request->message_id_.value(), request->token_, MessageType::UPDATE,
          ResponseCode::CHANGED));
    } else
      encoder_->encode(make_unique<Response>(
          request->endpoint_address_, request->endpoint_port_,
          request->message_id_.value(), request->token_, MessageType::UPDATE,
          ResponseCode::NOT_FOUND));
  } catch (ObjectDescriptorNotSupported &ex) {
    logger_->log(SeverityLevel::ERROR, ex.what());
    encoder_->encode(make_unique<ServerResponse_Register>(
        request->endpoint_address_, request->endpoint_port_,
        request->message_id_.value(), request->token_, MessageType::UPDATE,
        ResponseCode::BAD_REQUEST));
  }
  return true;
}

bool RegistryHandler::handleRequest(
    unique_ptr<ClientRequest_Deregister> request) {
  try {
    if (isRegistered(request->location_)) {
      device_registery_->erase(device_registery_->find(request->location_));
      notify(make_shared<RegistryEvent>(
          RegistryEvent{RegistryEventType::DEREGISTERED, request->location_,
                        shared_ptr<Device>()}));
      encoder_->encode(make_unique<Response>(
          request->endpoint_address_, request->endpoint_port_,
          request->message_id_.value(), request->token_,
          MessageType::DEREGISTER, ResponseCode::DELETED));
    } else
      encoder_->encode(make_unique<Response>(
          request->endpoint_address_, request->endpoint_port_,
          request->message_id_.value(), request->token_,
          MessageType::DEREGISTER, ResponseCode::NOT_FOUND));
  } catch (ObjectDescriptorNotSupported &ex) {
    logger_->log(SeverityLevel::ERROR, ex.what());
    encoder_->encode(make_unique<ServerResponse_Register>(
        request->endpoint_address_, request->endpoint_port_,
        request->message_id_.value(), request->token_, MessageType::DEREGISTER,
        ResponseCode::BAD_REQUEST));
  }
  return true;
}
} // namespace LwM2M