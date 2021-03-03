#include "Registrator.hpp"
#include "LoggerRepository.hpp"

using namespace std;
using namespace HaSLL;

namespace LwM2M {

ObjectDescriptorsMap Registrator::assignObjectDescriptors(
    const unordered_map<unsigned int, vector<unsigned int>>
        requested_object_instances) {
  logger_->log(SeverityLevel::TRACE, "Assigining Objct Descriptors");
  auto supported_object_descriptors = registry_->getSupportedDescriptors();
  ObjectDescriptorsMap result;
  for (auto requested_instance : requested_object_instances) {
    auto descriptor =
        supported_object_descriptors->find(requested_instance.first);
    if (descriptor != supported_object_descriptors->end()) {
      ObjectDescriptorPair instace_descriptor_pair =
          make_pair(descriptor->second, requested_instance.second);
      result.emplace(descriptor->first, move(instace_descriptor_pair));
    } else {
      logger_->log(SeverityLevel::WARNNING,
                   "Object id {} is not supported by the server.",
                   requested_instance.first);
    }
  }
  return result;
}

Registrator::Registrator(DeviceRegistryPtr registry,
                         RequestsManagerInterfacePtr requester)
    : registry_(registry), requester_(requester),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  if (!registry_) {
    throw invalid_argument("Device Registry can not be a nullptr.");
  }
  if (!requester_) {
    throw invalid_argument("Requester can not be a nullptr.");
  }
}

Registrator::~Registrator() {
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

RegisterResponsePtr Registrator::handleRquest(RegisterRequestPtr request) {
  if (request) {
    logger_->log(SeverityLevel::TRACE,
                 "Handling a Registration request from {}:{}",
                 request->endpoint_->endpoint_address_,
                 request->endpoint_->endpoint_port_);
    auto object_instances =
        assignObjectDescriptors(request->object_instances_map_);
    try {
      auto device = make_shared<Device>(
          requester_, request->endpoint_, object_instances, request->life_time_,
          request->endpoint_name_.value_or(string()), request->version_,
          request->binding_.value_or(BindingType::UDP),
          request->queue_mode_.value_or(false));
      auto location = registry_->registerDevice(device);
      return request->makeResponse(ResponseCode::CREATED, location);
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR,
                   "An unhandeled exception occured while handling "
                   "registration request. Exception: {}",
                   ex.what());
      return request->makeResponse(ResponseCode::BAD_REQUEST);
    }
  } else {
    throw invalid_argument("Register Request can not be a nullptr");
  }
}

UpdateResponsePtr Registrator::handleRquest(UpdateRequestPtr request) {
  if (request) {
    logger_->log(SeverityLevel::TRACE, "Handling an Update request from {}:{}",
                 request->endpoint_->endpoint_address_,
                 request->endpoint_->endpoint_port_);
    try {
      auto device = registry_->getDevice(request->location_);
      if (!request->object_instances_map_.empty()) {
        auto object_instances =
            assignObjectDescriptors(request->object_instances_map_);
        logger_->log(SeverityLevel::TRACE,
                     "Assiging new Object Instance map to {}:{} device.",
                     device->getDeviceId(), device->getName());
        device->updateObjectsMap(object_instances);
      }
      if (request->lifetime_.has_value()) {
        logger_->log(SeverityLevel::TRACE, "Updating lifetime of {}:{} device.",
                     device->getDeviceId(), device->getName());
        device->updateLifetime(request->lifetime_.value());
      }
      if (request->binding_.has_value()) {
        logger_->log(SeverityLevel::TRACE,
                     "Changing {}:{} device binding type to {}.",
                     device->getDeviceId(), device->getName(),
                     toString(request->binding_.value()));
        device->updateBinding(request->binding_.value());
      }
      if (request->sms_number_.has_value()) {
        logger_->log(SeverityLevel::ERROR, "SMS numbers are not supported!");
        return request->makeResponse();
      }
      registry_->updateDevice(device);
      return request->makeResponse(ResponseCode::CHANGED);
    } catch (DeviceNotFound &ex) {
      return request->makeResponse(ResponseCode::NOT_FOUND);
    }
  } else {
    throw invalid_argument("Update Request can not be a nullptr");
  }
}

DeregisterResponsePtr Registrator::handleRquest(DeregisterRequestPtr request) {
  if (request) {
    logger_->log(SeverityLevel::TRACE,
                 "Handling a Deregistration request from {}:{}",
                 request->endpoint_->endpoint_address_,
                 request->endpoint_->endpoint_port_);
    try {
      registry_->deregisterDevice(request->location_);
      return request->makeResponse(ResponseCode::DELETED);
    } catch (DeviceNotFound &ex) {
      return request->makeResponse(ResponseCode::NOT_FOUND);
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR,
                   "An unhandeled exception occured while handling "
                   "deregistration request. Exception: {}",
                   ex.what());
      return request->makeResponse();
    }
  } else {
    throw invalid_argument("Deregister Request can not be a nullptr");
  }
}

EventSourcePtr Registrator::getEventSource() { return registry_; }
} // namespace LwM2M
