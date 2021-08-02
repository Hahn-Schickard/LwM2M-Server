#include "Registrator.hpp"
#include "Discover.hpp"
#include "LoggerRepository.hpp"

#include <iterator>
#include <vector>

using namespace std;
using namespace HaSLL;

#define ERROR_CODES_VALUE 0x80

namespace LwM2M {

vector<DiscoverRequestPtr> makeDiscoverRequests(
    EndpointPtr endpoint,
    const RegisterRequest::ObjectInstancesMap object_instances) {
  vector<DiscoverRequestPtr> discover_requests;
  for (auto object_instance_pair : object_instances) {
    auto object_instance = ElementID(object_instance_pair.first);
    auto discover = make_shared<DiscoverRequest>(endpoint, object_instance);
    discover_requests.emplace_back(move(discover));
  }
  return move(discover_requests);
}

ElementIDs handleDiscoverResponse(ClientResponsePtr discover_response) {
  if (holds_alternative<ElementIDs>(discover_response->payload_->data_)) {
    return std::get<ElementIDs>(discover_response->payload_->data_);
  } else {
    string error_msg =
        "Request does not contain a vector of ObjectID instances";
    throw logic_error(error_msg);
  }
}

Registrator::Registrator(DeviceRegistryPtr registry)
    : registry_(registry),
      logger_(LoggerRepository::getInstance().registerTypedLoger(this)) {
  if (!registry_) {
    throw invalid_argument("Device Registry can not be a nullptr.");
  }
}

Registrator::~Registrator() {
  LoggerRepository::getInstance().deregisterLoger(logger_->getName());
}

ObjectDescriptorsMap
Registrator::assignAvailableDescriptors(ElementIDs requested_instances) {
  logger_->log(SeverityLevel::TRACE, "Assigning Available Descriptors");
  auto supported_object_descriptors = registry_->getSupportedDescriptors();
  ObjectDescriptorsMap result;
  for (auto object : requested_instances) {
    auto descriptor = supported_object_descriptors->find(object.getObjectID());
    if (descriptor != supported_object_descriptors->end()) {
      result.emplace(object, descriptor->second);
    } else {
      logger_->log(SeverityLevel::WARNNING,
                   "Object id {} is not supported by the server.",
                   object.getObjectID());
    }
  }
  return result;
}

ElementIDs Registrator::discoverAvailableDescriptors(
    EndpointPtr endpoint,
    const RegisterRequest::ObjectInstancesMap object_instances) {
  auto requests = makeDiscoverRequests(endpoint, object_instances);

  ElementIDs requested_instances;
  for (auto it = requests.begin(); it != requests.end();) {
    try {
      auto response_future = this->request(*it);
      auto response = response_future.get();
      if (static_cast<uint8_t>(response->response_code_) > ERROR_CODES_VALUE) {
        throw ResponseReturnedAnErrorCode(response, *it);
      } else {
        if (response->payload_) {
          auto resource_instances = handleDiscoverResponse(move(response));
          if (!resource_instances.empty()) {
            requested_instances.insert(requested_instances.end(),
                                       resource_instances.begin(),
                                       resource_instances.end());
          }
        } else {
          throw ResponseReturnedAnEmptyPayload(response, *it);
        }
      }
      ++it;
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR,
                   "Failed to handle {} to {}, due to an exception: {}. "
                   "Discarding it from available descriptors.",
                   (*it)->name(), (*it)->endpoint_->toString(), ex.what());
      it = requests.erase(it);
    }
  }
  return requested_instances;
}

RegisterResponsePtr Registrator::handleRquest(RegisterRequestPtr request) {
  if (request) {
    logger_->log(SeverityLevel::TRACE,
                 "Handling a Registration request from {}:{}",
                 request->endpoint_->endpoint_address_,
                 request->endpoint_->endpoint_port_);
    auto instances = discoverAvailableDescriptors(
        request->endpoint_, request->object_instances_map_);
    auto object_ids = assignAvailableDescriptors(instances);
    try {
      RequesterPtr requester = shared_from_this();
      auto device = make_shared<Device>(
          requester, request->endpoint_, object_ids, request->life_time_,
          request->endpoint_name_.value_or(string()), request->version_,
          request->binding_.value_or(BindingType::UDP),
          request->queue_mode_.value_or(false));
      auto location = registry_->registerDevice(device);
      return request->makeResponse(location);
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR,
                   "An unhandled exception occurred while handling "
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
        auto instances = discoverAvailableDescriptors(
            request->endpoint_, request->object_instances_map_);
        auto object_instances = assignAvailableDescriptors(instances);
        logger_->log(SeverityLevel::TRACE,
                     "Assigning new Object Instance map to {}:{} device.",
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
        return request->makeResponse(ResponseCode::BAD_REQUEST);
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
                   "An unhandled exception occurred while handling "
                   "deregistration request. Exception: {}",
                   ex.what());
      return request->makeResponse(ResponseCode::BAD_REQUEST);
    }
  } else {
    throw invalid_argument("Deregister Request can not be a nullptr");
  }
}

EventSourcePtr Registrator::getEventSource() { return registry_; }
} // namespace LwM2M
