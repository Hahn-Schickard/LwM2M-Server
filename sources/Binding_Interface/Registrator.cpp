#include "Registrator.hpp"

using namespace std;

namespace LwM2M {

ObjectDescriptorsMap Registrator::assignObjectDescriptors(
    const unordered_map<unsigned int, vector<unsigned int>>
        requested_object_instances) {
  auto supported_object_descriptors = registry_->getSupportedDescriptors();
  ObjectDescriptorsMap result;
  for (auto requested_instance : requested_object_instances) {
    auto descriptor =
        supported_object_descriptors->find(requested_instance.first);
    if (descriptor != supported_object_descriptors->end()) {
      ObjectDescriptorPair instace_descriptor_pair =
          make_pair(descriptor->second, requested_instance.second);
      result.emplace(descriptor->first, move(instace_descriptor_pair));
    }
  }
  return result;
}

Registrator::Registrator(DeviceRegistryPtr registry,
                         RequestsManagerInterfacePtr requester)
    : registry_(registry), requester_(requester) {
  if (!registry_) {
    throw invalid_argument("Device Registry can not be a nullptr.");
  }
  if (!requester_) {
    throw invalid_argument("Requester can not be a nullptr.");
  }
}

RegisterResponsePtr Registrator::handleRquest(RegisterRequestPtr request) {
  if (request) {
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
      // TODO: log exception here?
      return request->makeResponse(ResponseCode::BAD_REQUEST);
    }
  } else {
    throw invalid_argument("Register Request can not be a nullptr");
  }
}

UpdateResponsePtr Registrator::handleRquest(UpdateRequestPtr request) {
  if (request) {
    try {
      auto device = registry_->getDevice(request->location_);
      if (!request->object_instances_map_.empty()) {
        auto object_instances =
            assignObjectDescriptors(request->object_instances_map_);
        device->updateObjectsMap(object_instances);
      }
      if (request->lifetime_.has_value()) {
        device->updateLifetime(request->lifetime_.value());
      }
      if (request->binding_.has_value()) {
        device->updateBinding(request->binding_.value());
      }
      if (request->sms_number_.has_value()) {
        // TODO: implement endpoint updating
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
    try {
      registry_->deregisterDevice(request->location_);
      return request->makeResponse(ResponseCode::DELETED);
    } catch (DeviceNotFound &ex) {
      return request->makeResponse(ResponseCode::NOT_FOUND);
    } catch (exception &ex) {
      return request->makeResponse();
    }
  } else {
    throw invalid_argument("Deregister Request can not be a nullptr");
  }
}
} // namespace LwM2M
