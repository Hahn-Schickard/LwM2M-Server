#include "Registrator.hpp"
#include "Discover.hpp"
#include "LoggerRepository.hpp"
#include "Read.hpp"

#include <iterator>
#include <vector>

using namespace std;
using namespace HaSLL;

#define ERROR_CODES_VALUE 0x80
#define REQUEST_TIMEOUT_MS 2000

namespace LwM2M {

struct DiscoveryTimeout : public exception {
  const char *what() const noexcept override {
    return "Discovery request timed out!";
  }
};

ElementIDs &operator+=(ElementIDs &lhs, const ElementIDs &rhs) {
  if (!rhs.empty()) {
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
  }
  return lhs;
}

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

ReadRequestPtr makeReadRequest(DiscoverRequestPtr discover) {
  return make_shared<ReadRequest>(discover->endpoint_, discover->target_);
}

ElementIDs handleDiscoverResponse(ClientResponsePtr discover_response) {
  if (discover_response->payload_->hasData()) {
    if (holds_alternative<ElementIDs>(discover_response->payload_->data_)) {
      return std::get<ElementIDs>(discover_response->payload_->data_);
    } else {
      string error_msg =
          "Request does not contain a vector of ObjectID instances";
      throw logic_error(error_msg);
    }
  } else {
    string error_msg = "Discover response has no payload";
    throw logic_error(error_msg);
  }
}

ElementIDs handleReadResponse(ClientResponsePtr response,
                              ReadRequestPtr request) {
  ElementIDs result;
  if (response->response_code_ == ResponseCode::CONTENT) {
    if (response->payload_->hasData()) {
      if (holds_alternative<TargetContentVector>(response->payload_->data_)) {
        auto targets_and_values =
            std::get<TargetContentVector>(response->payload_->data_);
        for (auto target_value : targets_and_values) {
          // This is a very dumb way, but response does not have a valid object
          // id, since it is set in a request. Who needs redundancy anyways?
          auto id = ElementID(request->target_.getObjectID(),
                              target_value.first.getObjectInstanceID(),
                              target_value.first.getResourceID());
          result.push_back(id);
        }
      }
    } else {
      string error_msg = "Read response for manual discover has no payload";
      throw logic_error(error_msg);
    }
  } else {
    throw ResponseReturnedAnErrorCode(response, request);
  }
  return result;
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
  logger_->log(SeverityLevel::TRACE,
               "Trying to assing descriptors for {} elements.",
               requested_instances.size());
  for (auto object : requested_instances) {
    logger_->log(SeverityLevel::TRACE, "Looking for {} descriptor.",
                 object.toString());
    auto descriptor = supported_object_descriptors->find(object.getObjectID());
    if (descriptor != supported_object_descriptors->end()) {
      logger_->log(SeverityLevel::TRACE,
                   "Assigning {} object as a descriptor for {}",
                   descriptor->second->name_, object.toString());
      result.emplace(object, descriptor->second);
    } else {
      logger_->log(SeverityLevel::WARNNING,
                   "Object id {} is not supported by the server.",
                   object.getObjectID());
    }
  }
  return result;
}

ElementIDs Registrator::discover(ServerRequestPtr request) {
  auto response_future = this->request(request);
  if (response_future.wait_for(chrono::milliseconds(REQUEST_TIMEOUT_MS)) !=
      future_status::timeout) {
    auto response = response_future.get();
    if (static_cast<uint8_t>(response->response_code_) > ERROR_CODES_VALUE) {
      throw ResponseReturnedAnErrorCode(response, request);
    } else {
      if (response->payload_) {
        if (response->message_type_ == MessageType::DISCOVER) {
          return handleDiscoverResponse(move(response));
        } else {
          // we use request in case of failure code, to create and throw an
          // exception
          return handleReadResponse(move(response),
                                    static_pointer_cast<ReadRequest>(request));
        }
      } else {
        throw ResponseReturnedAnEmptyPayload(response, request);
      }
    }
  } else {
    try {
      logger_->log(SeverityLevel::INFO,
                   "{} for element {} to {} has timed out. Cancelling it.",
                   request->name(),
                   std::get<ElementID>(request->payload_->data_).toString(),
                   request->endpoint_->toString());
      this->cancelRequest(request);
      try {
        response_future.get(); // this must throw;
      } catch (exception & /*ex*/) {
        // safe to ignore, since canceled response, must throw an exception
      }
    } catch (exception &ex) {
      logger_->log(
          SeverityLevel::CRITICAL,
          "Could not cancel a discover request to {}, due to an exception: {}",
          request->endpoint_->toString(), ex.what());
    }
    throw DiscoveryTimeout();
  }
}

ElementIDs Registrator::discoverAvailableDescriptors(
    EndpointPtr endpoint,
    const RegisterRequest::ObjectInstancesMap object_instances) {
  auto requests = makeDiscoverRequests(endpoint, object_instances);

  ElementIDs requested_instances;
  for (auto it = requests.begin(); it != requests.end();) {
    try {
      logger_->log(SeverityLevel::INFO, "Discovering object {} from {}",
                   (*it)->target_.toString(), (*it)->endpoint_->toString());
      auto pre_discover_size = requested_instances.size();
      requested_instances += discover(*it);
      logger_->log(SeverityLevel::TRACE, "Discovered {} elements for target {}",
                   requested_instances.size() - pre_discover_size,
                   (*it)->target_.toString());
    }
    // @TODO: handle method not allowed and similar exceptions
    catch (DiscoveryTimeout & /*timeout*/) {
      try {
        logger_->log(SeverityLevel::INFO,
                     "Discovery for object {} from {} has timedout. Doing a "
                     "manual read.",
                     (*it)->target_.toString(), (*it)->endpoint_->toString());
        requested_instances += discover(makeReadRequest(*it));
      } catch (DiscoveryTimeout & /*timeout*/) {
        logger_->log(
            SeverityLevel::WARNNING,
            "Manual discovery for {} object {} failed due to a message "
            "timeout. Discarding it from available descriptors.",
            (*it)->endpoint_->toString(), (*it)->target_.toString());
      } catch (exception &ex) {
        logger_->log(
            SeverityLevel::ERROR,
            "Manual discovery for {} object {} failed due to an exception: {}. "
            "Discarding it from available descriptors.",
            (*it)->endpoint_->toString(), (*it)->target_.toString(), ex.what());
      }
    } catch (exception &ex) {
      logger_->log(SeverityLevel::ERROR,
                   "Failed to handle {} to {}, due to an exception: {}. "
                   "Discarding it from available descriptors.",
                   (*it)->name(), (*it)->endpoint_->toString(), ex.what());
    }
    ++it;
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
