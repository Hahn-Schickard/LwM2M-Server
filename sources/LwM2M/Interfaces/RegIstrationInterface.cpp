#include "RegistrationInterface.hpp"
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

RegistrationInterface::RegistrationInterface(
    shared_ptr<unordered_map<string, shared_ptr<Device>>> device_registery,
    shared_ptr<MessageEncoder> encoder, const string &configuration_path)
    : device_registery_(device_registery), encoder_(encoder),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  try {
    supported_descriptors_ = deserializeModel(configuration_path);
  } catch (exception &ex) {
    logger_->log(SeverityLevel::ERROR,
                 "Received an exception during descriptor deserialization: {}",
                 ex.what());
  }
}

unordered_map<uint32_t, shared_ptr<ObjectDescriptor>>
RegistrationInterface::assignObjectInstances(
    unordered_map<unsigned int, unsigned int> objects) {
  unordered_map<uint32_t, shared_ptr<ObjectDescriptor>> result;
  for (auto &object : objects) {
    uint32_t object_id = object.first;
    uint32_t instance_id = object.second;
    auto it = supported_descriptors_.find(object_id);
    if (it != supported_descriptors_.end()) {
      result.emplace(instance_id, it->second);
    } else {
      string error_msg = "Object ID " + to_string(object_id) +
                         " is not supported by the server.";
      throw ObjectDescriptorNotSupported(move(error_msg));
    }
  }
  return result;
}

bool RegistrationInterface::isRegistered(string device_id) {
  return (device_registery_->find(device_id) != device_registery_->end())
             ? true
             : false;
}

bool RegistrationInterface::handleRequest(
    unique_ptr<Register_Request> request) {
  unique_ptr<Register_Response> result;
  try {
    unordered_map<uint32_t, shared_ptr<ObjectDescriptor>> object_instances =
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
    result = make_unique<Register_Response>(
        request->endpoint_address_, request->endpoint_port_,
        request->message_id_, request->token_, MessageType::REGISTER,
        ResponseCode::CREATED, vector<string>{"rd", new_device->getDeviceId()});
  } catch (ObjectDescriptorNotSupported &ex) {
    logger_->log(SeverityLevel::ERROR, ex.what());
    result = make_unique<Register_Response>(
        request->endpoint_address_, request->endpoint_port_,
        request->message_id_, request->token_, MessageType::REGISTER,
        ResponseCode::BAD_REQUEST);
  }
  encoder_->encode(move(result));
  return true;
} // namespace LwM2M

bool RegistrationInterface::handleRequest(unique_ptr<Update_Request> request) {
  bool result = false;
  return result;
}

bool RegistrationInterface::handleRequest(
    unique_ptr<Deregister_Request> request) {
  bool result = false;
  return result;
}
} // namespace LwM2M